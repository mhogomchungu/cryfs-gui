/*
 *
 *  Copyright (c) 2014-2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZULUMOUNTTASK_H
#define ZULUMOUNTTASK_H

#include "volumeentryproperties.h"
#include "task.h"
#include "utility.h"

#include <QVector>
#include <QString>
#include <QStringList>

namespace cryfsGUITask
{
	struct encryptedVolume
	{
		enum class status
		{
			success,cryfs,cryfsNotFound,encfs,encfsNotFound,unknown,failedToCreateMountPoint,backendFail
		}state;
	};

	Task::future< QVector< volumeEntryProperties > >& updateVolumeList( void ) ;
	Task::future< bool >& encryptedFolderUnMount( const QString& mountPoint ) ;
	Task::future< encryptedVolume >& encryptedFolderMount( const QString& volumePath,const QString& mountPoint,const QString& key,bool ro ) ;
}

#endif // ZULUMOUNTTASK_H
