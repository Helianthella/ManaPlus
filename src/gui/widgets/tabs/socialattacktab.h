/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_WIDGETS_TABS_SOCIALATTACKTAB_H
#define GUI_WIDGETS_TABS_SOCIALATTACKTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "being/localplayer.h"

#include "gui/models/beingslistmodel.h"

#include "gui/widgets/tabs/socialtabbase.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "localconsts.h"

class SocialAttackTab final : public SocialTab
{
    public:
        SocialAttackTab(const Widget2 *const widget,
                        const bool showBackground) :
            SocialTab(widget),
            mBeings(new BeingsListModel)
        {
            createControls(mBeings, showBackground);

            // TRANSLATORS: Attack filter tab name in social window.
            // TRANSLATORS: Should be small
            setCaption(_("Atk"));
        }

        A_DELETE_COPY(SocialAttackTab)

        ~SocialAttackTab()
        {
            delete2(mList)
            delete2(mScroll)
            delete2(mBeings)
        }

        void updateList() override final
        {
            updateAtkListStart();
            // TRANSLATORS: mobs group name in social window
            addAvatars(PriorityAttackMob, _("Priority mobs"), PRIORITY);
            // TRANSLATORS: mobs group name in social window
            addAvatars(AttackMob, _("Attack mobs"), ATTACK);
            // TRANSLATORS: mobs group name in social window
            addAvatars(IgnoreAttackMob, _("Ignore mobs"), IGNORE_);
        }

    private:
        BeingsListModel *mBeings;
};

#endif  // GUI_WIDGETS_TABS_SOCIALATTACKTAB_H
