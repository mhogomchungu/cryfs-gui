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

using cs = cryfsTask::status ;

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

bool cryfsTask::UnmountEncryptedFolder( const QString& m )
{
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
}

Task::future< bool >& cryfsTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		return cryfsTask::UnmountEncryptedFolder( m ) ;
	} ) ;
}

static cs _cmd( const QString& e,cs status,const QString& arguments,const QString& k )
{
	QString exe ;

	for( const auto& it : { "/usr/local/bin/","/usr/local/sbin/","/usr/bin/","/usr/sbin/" } ){

		exe = it + e ;

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

					return cs::success ;
				}else{
					return status ;
				}
			}else{
				return cs::backendFail ;
			}
		}
	}

	if( status == cs::cryfs ){

		return cs::cryfsNotFound ;
	}else{
		return cs::encfsNotFound ;
	}
}

struct arguments
{
	const cryfsTask::options& opt ;
	const QString& type ;
	const QString& arguments ;
	const QString& m_point ;
};

static QString _args( const arguments& args )
{
	auto c = args.opt.cipherFolder ;
	c.replace( "\"","\"\"\"" ) ;

	QString m ;

	if( args.m_point.isEmpty() ){

		m = args.opt.plainFolder ;
	}else{
		m = args.m_point ;
	}

	m.replace( "\"","\"\"\"" ) ;

	const char * opts ;

	if( args.opt.ro ){

		opts = "\"%1\" \"%2\" %3 -o ro -o fsname=%4@\"%5\" -o subtype=%6" ;
	}else{
		opts = "\"%1\" \"%2\" %3 -o rw -o fsname=%4@\"%5\" -o subtype=%6" ;
	}

	return QString( opts ).arg( c,m,args.arguments,args.type,c,args.type ) ;
}

Task::future< cs >& cryfsTask::encryptedFolderMount( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		auto _mount = [ & ]( std::function< cs() >&& unlock ){

			if( _create_mount_point( opt.plainFolder ) ){

				auto e = unlock() ;

				if( e != cs::success ) {

					_delete_mount_point( opt.plainFolder ) ;
				}

				return e ;
			}else{
				return cs::failedToCreateMountPoint ;
			}
		} ;

		if( utility::pathExists( opt.cipherFolder + "/cryfs.config" ) ){

			return _mount( [ & ](){

				return _cmd( "cryfs",cs::cryfs,_args( { opt,"cryfs","--","" } ),opt.key ) ;
			} ) ;
		}

		auto encfs6 = opt.cipherFolder + "/.encfs6.xml" ;
		auto encfs5 = opt.cipherFolder + "/.encfs5" ;
		auto encfs4 = opt.cipherFolder + "/.encfs4" ;

		if( utility::atLeastOnePathExists( encfs6,encfs5,encfs4 ) ){

			return _mount( [ & ](){

				return _cmd( "encfs",cs::encfs,_args( { opt,"encfs","-S","" } ),opt.key ) ;
			} ) ;
		}

		return cs::unknown ;
	} ) ;
}

Task::future< cs >& cryfsTask::encryptedFolderCreate( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		if( _create_mount_point( opt.cipherFolder ) ){

			auto m = utility::mountPath( utility::mountPathPostFix( opt.plainFolder ) ) ;

			if( _create_mount_point( m ) ){

				auto e = _cmd( "cryfs",cs::cryfs,_args( { opt,"cryfs","--",m } ),opt.key ) ;

				if( e == cs::success ){

					opt.openFolder( m ) ;
				}else{
					_delete_mount_point( m ) ;
					_delete_mount_point( opt.cipherFolder ) ;
				}

				return e ;
			}else{
				_delete_mount_point( opt.cipherFolder ) ;

				return cs::failedToCreateMountPoint ;
			}
		}else{
			return cs::failedToCreateMountPoint ;
		}
	} ) ;
}

Task::future< QVector< volumeInfo > >& cryfsTask::updateVolumeList()
{
	return Task::run< QVector< volumeInfo > >( [](){

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

		auto _decode = []( QString path,bool set_offset ){

			path.replace( "\\012","\n" ) ;
			path.replace( "\\040"," " ) ;
			path.replace( "\\134","\\" ) ;
			path.replace( "\\011","\\t" ) ;

			if( set_offset ){

				path = path.mid( 6 ) ;
			}

			return path ;
		} ;

		auto _fs = []( const QString& e ){

			return QString( e.toLatin1().constData() + 5 ) ;
		} ;

		auto _ro = []( const QStringList& l ){

			const auto& e = l.at( 5 ) ;

			if( utility::containsAtleastOne( e,"ro,",",ro",",ro," ) ){

				return "ro" ;
			}else{
				return "rw" ;
			}
		} ;

		QVector< volumeInfo > e ;

		for( const auto& it : utility::monitor_mountinfo::mountinfo() ){

			if( utility::containsAtleastOne( it," fuse.cryfs "," fuse.encfs " ) ){

				const auto& k = utility::split( it,' ' ) ;

				const auto s = k.size() ;

				const auto& cf = k.at( s - 2 ) ;

				const auto& m = k.at( 4 ) ;

				const auto& fs = k.at( s - 3 ) ;

				if( utility::startsWithAtLeastOne( cf,"encfs@","cryfs@" ) ){

					e.append( { _decode( cf,true ),_decode( m,false ),_fs( fs ),_ro( k ) } ) ;
				}else{
					e.append( { _hash( m ),_decode( m,false ),_fs( fs ),_ro( k ) } ) ;
				}
			}
		}

		return e ;
	} ) ;
}
