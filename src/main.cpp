/*
 * This file is part of the Kvkbd project.
 * Copyright (C) 2007-2014 Todor Gyumyushev <yodor1@gmail.com>
 * Copyright (C) 2008 Guillaume Martres <smarter@ubuntu.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */



#include "kvkbdapp.h"
#include <KAboutData>
#include <KCmdLineArgs>

static const char description[] =
    I18N_NOOP("A virtual keyboard for KDE");

static const char version[] = "0.7.0";


int main(int argc, char **argv)
{

    KAboutData about("kvkbd", 0, ki18n("Kvkbd"), version, ki18n(description),
                     KAboutData::License_LGPL_V3, ki18n("(C) 2007-2014 The Kvkbd Developers"));
    about.addAuthor(ki18n("Todor Gyumyushev"), ki18n("Original Author"), "yodor1@gmail.com");
    about.addAuthor(ki18n("Guillaume Martres"), ki18n("KDE4 port"), "smarter@ubuntu.com");
    about.setProgramIconName("preferences-desktop-keyboard");

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineOptions options;

    options.add("loginhelper", ki18n("Stand alone version for use with KDM or XDM.\n"
                                     "See Kvkbd Handbook for information on how to use this option."));
    KCmdLineArgs::addCmdLineOptions(options);

    KvkbdApp app(argc, argv);

    return app.exec();
    
}



