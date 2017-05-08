/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2017 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#include "UserAgentPropertiesDialog.h"
#include "../../core/AddonsManager.h"
#include "../../core/ThemesManager.h"
#include "../../core/WebBackend.h"

#include "ui_UserAgentPropertiesDialog.h"

#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolTip>

namespace Otter
{

UserAgentPropertiesDialog::UserAgentPropertiesDialog(const UserAgentDefinition &userAgent, QWidget *parent) : Dialog(parent),
	m_userAgent(userAgent),
	m_ui(new Ui::UserAgentPropertiesDialog)
{
	m_ui->setupUi(this);
	m_ui->titleLineEdit->setText(userAgent.getTitle());
	m_ui->valueLineEdit->setText(userAgent.value);
	m_ui->previewButton->setIcon(ThemesManager::createIcon(QLatin1String("document-preview")));

	if (userAgent.identifier == QLatin1String("default"))
	{
		m_ui->valueLineEdit->setReadOnly(true);
	}
	else
	{
		m_ui->valueLineEdit->installEventFilter(this);
	}

	setWindowTitle(userAgent.isValid() ? tr ("Edit User Agent") : tr("Add User Agent"));

	connect(m_ui->previewButton, SIGNAL(clicked(bool)), this, SLOT(showPreview()));
}

UserAgentPropertiesDialog::~UserAgentPropertiesDialog()
{
	delete m_ui;
}

void UserAgentPropertiesDialog::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);

	if (event->type() == QEvent::LanguageChange)
	{
		m_ui->retranslateUi(this);
	}
}

void UserAgentPropertiesDialog::insertPlaceholder(QAction *action)
{
	if (!action->data().toString().isEmpty())
	{
		m_ui->valueLineEdit->insert(QStringLiteral("{%1}").arg(action->data().toString()));
	}
}

void UserAgentPropertiesDialog::showPreview()
{
	QToolTip::showText(m_ui->valueLineEdit->mapToGlobal(m_ui->valueLineEdit->rect().bottomLeft()), AddonsManager::getWebBackend()->getUserAgent(m_ui->valueLineEdit->text()));
}

UserAgentDefinition UserAgentPropertiesDialog::getUserAgent() const
{
	UserAgentDefinition userAgent(m_userAgent);
	userAgent.title = m_ui->titleLineEdit->text();
	userAgent.value = m_ui->valueLineEdit->text();

	return userAgent;
}

bool UserAgentPropertiesDialog::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::ContextMenu)
	{
		QLineEdit *lineEdit(qobject_cast<QLineEdit*>(object));

		if (lineEdit)
		{
			QMenu *contextMenu(lineEdit->createStandardContextMenu());
			contextMenu->addSeparator();

			QMenu *placeholdersMenu(contextMenu->addMenu(tr("Placeholders")));
			placeholdersMenu->addAction(tr("Platform"))->setData(QLatin1String("platform"));
			placeholdersMenu->addAction(tr("Engine Version"))->setData(QLatin1String("engineVerion"));
			placeholdersMenu->addAction(tr("Application Version"))->setData(QLatin1String("applicationVersion"));

			connect(placeholdersMenu, SIGNAL(triggered(QAction*)), this, SLOT(insertPlaceholder(QAction*)));

			contextMenu->exec(static_cast<QContextMenuEvent*>(event)->globalPos());
			contextMenu->deleteLater();

			return true;
		}
	}

	return QDialog::eventFilter(object, event);
}

}