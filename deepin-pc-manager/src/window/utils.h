/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     lq <longqi_cm@deepin.com>
 *
 * Maintainer: lq <longqi_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef V20_DISPLAY_UTILS_H
#define V20_DISPLAY_UTILS_H

#include "window/namespace.h"

#include <QMargins>
#include <QVariant>

Q_DECLARE_METATYPE(QMargins)

DEF_NAMESPACE_BEGIN

//const QMargins ThirdPageContentsMargins(10, 10, 10, 10);
const QMargins ThirdPageContentsMargins(0, 0, 0, 0);
const QMargins ListViweItemMargin(10, 8, 10, 8);
const QVariant VListViewItemMargin = QVariant::fromValue(ListViweItemMargin);
const QMargins ListViweRightSubscriptItemMargin(42, 0, 0, 8);
const QVariant VListViewRightSubscripItemMargin = QVariant::fromValue(ListViweRightSubscriptItemMargin);
const int List_Interval = 10;
DEF_NAMESPACE_END

#endif // V20_DISPLAY_UTILS_H
