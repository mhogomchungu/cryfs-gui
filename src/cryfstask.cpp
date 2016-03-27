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

			auto mm = m ;
			mm.replace( "\"","\"\"\"" ) ;

			if( utility::Task( "fusermount -u \"" + mm + "\"",10000 ).success() ){

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

		auto _mount = [ & ]( std::function< ev() >&& unlocked )->ev{

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

		auto pp = p ;
		pp.replace( "\"","\"\"\"" ) ;

		auto mm = m ;
		mm.replace( "\"","\"\"\"" ) ;

		if( utility::pathExists( p + "/cryfs.config" ) ){

			return _mount( [ & ](){

				const char * opts ;

				if( ro ){

					opts = "\"%1\" \"%2\" -- -o ro -o fsname=cryfs@\"%3\" -o subtype=cryfs" ;
				}else{
					opts = "\"%1\" \"%2\" -- -o rw -o fsname=cryfs@\"%3\" -o subtype=cryfs" ;
				}

				return _cmd( "cryfs",ev::status::cryfs,QString( opts ).arg( pp,mm,pp ),k ) ;
			} ) ;
		}

		if( utility::pathExists( p + "/.encfs6.xml" ) ){

			return _mount( [ & ](){

				const char * opts ;

				if( ro ){

					opts = "\"%1\" \"%2\" -S -o ro -o fsname=encfs@\"%3\" -o subtype=encfs" ;
				}else{
					opts = "\"%1\" \"%2\" -S -o rw -o fsname=encfs@\"%3\" -o subtype=encfs" ;
				}

				return _cmd( "encfs",ev::status::encfs,QString( opts ).arg( pp,mm,pp ),k ) ;
			} ) ;
		}

		return { ev::status::unknown } ;
	} ) ;
}

Task::future< cryfsTask::encryptedVolume >&
cryfsTask::encryptedFolderCreate( const QString& cipherFolder,
				  const QString& plainFoder,
				  const QString& key,
				  std::function< void( const QString& )> openFolder )
{
	return Task::run< ev >( [ cipherFolder,plainFoder,key,openFolder ]()->ev{

		if( _create_mount_point( cipherFolder ) ){

			auto m_point = utility::mountPath( utility::mountPathPostFix( plainFoder ) ) ;

			if( _create_mount_point( m_point ) ){

				auto m = m_point ;
				m.replace( "\"","\"\"\"" ) ;

				auto c = cipherFolder ;
				c.replace( "\"","\"\"\"" ) ;

				auto opts = "\"%1\" \"%2\" -- -o rw -o fsname=cryfs@\"%3\" -o subtype=cryfs" ;

				auto e = _cmd( "cryfs",ev::status::cryfs,QString( opts ).arg( c,m,c ),key ) ;

				if( e.state == ev::status::success ){

					openFolder( m_point ) ;
				}else{
					_delete_mount_point( m_point ) ;
					_delete_mount_point( cipherFolder ) ;
				}

				return e ;
			}else{
				_delete_mount_point( cipherFolder ) ;

				return { ev::status::failedToCreateMountPoint } ;
			}
		}else{
			return { ev::status::failedToCreateMountPoint } ;
		}
	} ) ;
}

Task::future< QVector< volumeEntryProperties > >& cryfsTask::updateVolumeList()
{
	return Task::run< QVector< volumeEntryProperties > >( [](){

		auto _hash = []( const QString& e ){

			uint32_t hash = 0 ;

			auto p = e.toLatin1() ;

			auto key = p.constData() ;

			auto l = p.size() ;

			for( decltype( l ) i = 0 ; i < l ; i++ ){

				hash += *( key + i ) ;

				hash += ( hash << 10 ) ;

				hash ^= ( hash >> 6 ) ;
			}

			hash += ( hash << 3 ) ;

			hash ^= ( hash >> 11 ) ;

			hash += ( hash << 15 ) ;

			return QString::number( hash ) ;
		} ;

		auto _decode_entry = []( QString path,bool set_offset ){

			path.replace( "\\012","\n" ) ;
			path.replace( "\\012","\n" ) ;
			path.replace( "\\040"," " ) ;
			path.replace( "\\134","\\" ) ;
			path.replace( "\\011","\\t" ) ;

			if( set_offset ){

				path = path.mid( 6 ) ;
			}

			return path ;
		} ;

		QVector< volumeEntryProperties > e ;

		for( const auto& it : utility::monitor_mountinfo::mountinfo() ){

			if( it.contains( " fuse.cryfs " ) || it.contains( " fuse.encfs " ) ){

				const auto k = utility::split( it,' ' ) ;

				const auto s = k.size() ;

				const auto& cipher_folder = k.at( s - 2 ) ;

				const auto& mount_point = k.at( 4 ) ;

				const auto& fs = k.at( s - 3 ) ;

				if( cipher_folder.startsWith( "encfs@" ) ){

					e.append( { _decode_entry( cipher_folder,true ),
						    _decode_entry( mount_point,false ),
						    "encfs",
						    "Nil","Nil","Nil" } ) ;

				}else if( cipher_folder.startsWith( "cryfs@" ) ){

					e.append( { _decode_entry( cipher_folder,true ),
						    _decode_entry( mount_point,false ),
						    "cryfs",
						    "Nil","Nil","Nil" } ) ;
				}else{
					e.append( { _hash( mount_point ),
						    _decode_entry( mount_point,false ),
						    fs.toLatin1().constData() + 5,
						    "Nil","Nil","Nil" } ) ;
				}
			}
		}

		return e ;
	} ) ;
}
