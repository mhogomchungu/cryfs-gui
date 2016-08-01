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

#include "volumeinfo.h"
#include "task.h"
#include "utility.h"

#include <QVector>
#include <QString>
#include <QStringList>

namespace cryfsTask
{
	struct options
	{
		QString cipherFolder ;
		QString plainFolder ;
		QString key ;
		QString mOpt ;
                QString configFilePath ;
		QString exe ;
		bool ro ;
		std::function< void( const QString& ) > openFolder ;
	};

	enum class status
	{
		success,
		cryfs,
		encfs,
		gocryptfs,
		securefs,
		gocryptfsNotFound,
		cryfsNotFound,
		encfsNotFound,
		securefsNotFound,
		unknown,
		failedToCreateMountPoint,
		backendFail
	};

	bool deleteMountFolder( const QString& ) ;
	Task::future< QVector< volumeInfo > >& updateVolumeList( void ) ;
	Task::future< bool >& encryptedFolderUnMount( const QString& mountPoint ) ;
	Task::future< cryfsTask::status >& encryptedFolderMount( const options&,bool = false ) ;
	Task::future< cryfsTask::status >& encryptedFolderCreate( const options& ) ;
}

#endif // ZULUMOUNTTASK_H
