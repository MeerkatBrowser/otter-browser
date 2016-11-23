/**************************************************************************
* Meerkat Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2016 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
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

#ifndef MEERKAT_PROGRESSINFORMATIONWIDGET_H
#define MEERKAT_PROGRESSINFORMATIONWIDGET_H

#include "../../../core/ActionsManager.h"
#include "../../../ui/WebWidget.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

namespace Meerkat
{

class Window;

class ProgressInformationWidget : public QWidget
{
	Q_OBJECT

public:
	enum ProgressInformationType
	{
		UnknownType = 0,
		DocumentProgressType,
		TotalSizeType,
		ElementsType,
		SpeedType,
		ElapsedTimeType,
		MessageType
	};

	explicit ProgressInformationWidget(Window *window, const ActionsManager::ActionEntryDefinition &definition, QWidget *parent = nullptr);

	QSize sizeHint() const;

protected slots:
	void updateStatus(WebWidget::PageInformation key, const QVariant &value = QVariant());
	void setWindow(Window *window);

private:
	Window *m_window;
	QLabel *m_label;
	QProgressBar *m_progressBar;
	ProgressInformationType m_type;
};

}

#endif
