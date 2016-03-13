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

#include "cryfstask.h"

#include <QDir>
#include <QString>
#include <QDebug>
#include <QFile>

#include <stdlib.h>

static bool _delete_mount_point( const QString& m )
{
	QDir e;
	e.rmdir( m ) ;
	return true ;
}

static bool _create_mount_point( const QString& m )
{
	if( utility::pathExists( m ) ){

		return false ;
	}else{
		QDir e( m ) ;
		return e.mkpath( m ) ;
	}
}

Task::future<bool>& cryfsTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		auto _umount = [ & ](){

			if( utility::Task( "fusermount -u \"" + m + "\"",10000 ).success() ){

				return _delete_mount_point( m ) ;
			}else{
				return false ;
			}
		} ;

		utility::Task::waitForOneSecond() ;

		for( int i = 0 ; i < 5 ; i++ ){

			if( _umount() ){

				return true ;
			}else{
				utility::Task::waitForOneSecond() ;
			}
		}

		return false ;
	} ) ;
}

using ev = cryfsTask::encryptedVolume ;

static ev _cmd( const QString& e,ev::status status,const QString& arguments,const QString& k )
{
	for( const auto& it : { "/usr/local/bin/","/usr/local/sbin/","/usr/bin/","/usr/sbin/" } ){

		auto exe = it + e ;

		if( utility::pathExists( exe ) ){

			setenv( "CRYFS_NO_UPDATE_CHECK","TRUE",1 ) ;
			setenv( "CRYFS_FRONTEND","noninteractive",1 ) ;

			QProcess e ;

			e.start( exe + " " + arguments ) ;

			e.waitForStarted() ;

			e.write( k.toLatin1() + '\n' ) ;

			e.closeWriteChannel() ;

			if( e.waitForFinished( 20000 ) ){

				if( e.exitCode() == 0 ){

					return { ev::status::success } ;
				}else{
					return { status } ;
				}
			}else{
				return { ev::status::backendFail } ;
			}
		}
	}

	if( status == ev::status::cryfs ){

		return { ev::status::cryfsNotFound } ;
	}else{
		return { ev::status::encfsNotFound } ;
	}
}

Task::future< ev >& cryfsTask::encryptedFolderMount( const QString& p,const QString& m,const QString& k,bool ro )
{
	return Task::run< ev >( [ p,m,k,ro ]()->ev{

		auto _mount = [ & ]( std::function< ev() > unlocked )->ev{

			if( _create_mount_point( m ) ){

				auto e = unlocked() ;

				if( e.state != ev::status::success ) {

					_delete_mount_point( m ) ;
				}

				return e ;
			}else{
				return { ev::status::failedToCreateMountPoint } ;
			}
		} ;

		if( utility::pathExists( p + "/cryfs.config" ) ){

			return _mount( [ & ](){

				const char * opts ;

				if( ro ){

					opts = "%1 %2 -- -o ro -o fsname=cryfs@%3 -o subtype=cryfs" ;
				}else{
					opts = "%1 %2 -- -o rw -o fsname=cryfs@%3 -o subtype=cryfs" ;
				}

				return _cmd( "cryfs",ev::status::cryfs,QString( opts ).arg( p,m,p ),k ) ;
			} ) ;
		}

		if( utility::pathExists( p + "/.encfs6.xml" ) ){

			return _mount( [ & ](){

				const char * opts ;

				if( ro ){

					opts = "%1 %2 -S -o ro -o fsname=encfs@%3 -o subtype=encfs" ;
				}else{
					opts = "%1 %2 -S -o rw -o fsname=encfs@%3 -o subtype=encfs" ;
				}

				return _cmd( "encfs",ev::status::encfs,QString( opts ).arg( p,m,p ),k ) ;
			} ) ;
		}

		return { ev::status::unknown } ;
	} ) ;
}

Task::future< cryfsTask::encryptedVolume >& cryfsTask::encryptedFolderCreate( const QString& cipherFolder,
									      const QString& plainFoder,
									      const QString& key,
									      std::function< void( const QString& )> openFolder )
{
	return Task::run< ev >( [ cipherFolder,plainFoder,key,openFolder ]()->ev{

		if( _create_mount_point( cipherFolder ) ){

			auto m_point = utility::homePath() + "/" + utility::mountPathPostFix( plainFoder ) ;

			if( _create_mount_point( m_point ) ){

				auto opts = "%1 %2 -- -o rw -o fsname=cryfs@%3 -o subtype=cryfs" ;

				auto e = _cmd( "cryfs",ev::status::cryfs,QString( opts ).arg( cipherFolder,m_point,cipherFolder ),key ) ;

				if( e.state == ev::status::success ){

					openFolder( m_point ) ;
				}

				return e ;
			}else{
				return { ev::status::failedToCreateMountPoint } ;
			}
		}else{
			return { ev::status::failedToCreateMountPoint } ;
		}
	} ) ;
}

Task::future< QVector<volumeEntryProperties > >& cryfsTask::updateVolumeList()
{
	return Task::run< QVector< volumeEntryProperties > >( [](){

		QVector< volumeEntryProperties > e ;

		for( const auto& it : utility::monitor_mountinfo::mountinfo() ){

			if( it.contains( " fuse.cryfs " ) || it.contains( " fuse.encfs " ) ){

				auto k = utility::split( it,' ' ) ;

				auto s = k.size() ;

				auto plainFolder = k.at( s - 2 ) ;

				const auto& cipherFolder = k.at( 4 ) ;

				if( plainFolder.startsWith( "encfs@" ) ){

					plainFolder = plainFolder.mid( 6 ) ;

					e.append( { plainFolder,cipherFolder,"encfs","Nil","Nil","Nil" } ) ;

				}else if( plainFolder.startsWith( "cryfs@" ) ){

					plainFolder = plainFolder.mid( 6 ) ;

					e.append( { plainFolder,cipherFolder,"cryfs","Nil","Nil","Nil" } ) ;
				}
			}
		}

		return e ;
	} ) ;
}
