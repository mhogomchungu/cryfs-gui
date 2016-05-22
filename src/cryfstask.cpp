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

static bool _delete_folder( const QString& m )
{
	if( !utility::reUseMountPoint() ){

		QDir e;
		e.rmdir( m ) ;
	}

	return true ;
}

static bool _create_folder( const QString& m )
{
	if( utility::pathExists( m ) ){

		return utility::reUseMountPoint() ;
	}else{
		QDir e ;
		return e.mkpath( m ) ;
	}
}

Task::future< bool >& cryfsTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		auto _umount = [ & ](){

			auto mm = m ;
			mm.replace( "\"","\"\"\"" ) ;

			if( utility::Task( "fusermount -u \"" + mm + "\"",10000 ).success() ){

				return _delete_folder( m ) ;
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

static cs _cmd( const QString& app,const cryfsTask::options& opt )
{	
	auto _args = []( const QString& exe,const cryfsTask::options& opt,const QString& type ){

		auto cipherFolder = opt.cipherFolder ;
		cipherFolder.replace( "\"","\"\"\"" ) ;

		auto mountPoint = opt.plainFolder ;

		mountPoint.replace( "\"","\"\"\"" ) ;

		auto mountOptions = [ & ](){

			if( !opt.mOpt.isEmpty() ){

				if( type == "encfs" ){

					return QString( "--unmount-idle %1" ).arg( opt.mOpt ) ;

				}else if( type == "encfs" ){

					return QString( "--idle=%1" ).arg( opt.mOpt ) ;
				}
			}

			return QString() ;
		}() ;

		auto separator = [ & ](){

			if( type == "cryfs" ){

				return "--" ;

			}else if( type == "encfs" ){

				return "-S" ;
			}else{
				return "" ;
			}
		}() ;

		auto o = [ & ]()->QString{

			if( opt.ro ){

				return "%1 \"%2\" \"%3\" %4 %5 -o ro -o fsname=%6@\"%7\" -o subtype=%8" ;
			}else{
				return "%1 \"%2\" \"%3\" %4 %5 -o rw -o fsname=%6@\"%7\" -o subtype=%8" ;
			}
		}() ;

		return o.arg( exe,cipherFolder,mountPoint,mountOptions,separator,type,cipherFolder,type ) ;
	} ;

	auto exe = utility::executableFullPath( app ) ;

	if( exe.isEmpty() ){

		if( app == "cryfs" ){

			return cs::cryfsNotFound ;
		}else{
			return cs::encfsNotFound ;
		}
	}else{
		setenv( "CRYFS_NO_UPDATE_CHECK","TRUE",1 ) ;
		setenv( "CRYFS_FRONTEND","noninteractive",1 ) ;

		QProcess e ;

		e.start( _args( exe,opt,app ) ) ;

		e.waitForStarted() ;

		e.write( opt.key.toLatin1() + '\n' ) ;

		e.closeWriteChannel() ;

		if( e.waitForFinished( 20000 ) ){

			if( e.exitCode() == 0 ){

				return cs::success ;
			}else{
				if( app == "cryfs" ){

					return cs::cryfs ;
				}else{
					return cs::encfs ;
				}
			}
		}else{
			return cs::backendFail ;
		}
	}	
}

Task::future< cs >& cryfsTask::encryptedFolderMount( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		auto _mount = []( const QString& app,const options& opt ){

			if( _create_folder( opt.plainFolder ) ){

				auto e = _cmd( app,opt ) ;

				if( e != cs::success ) {

					_delete_folder( opt.plainFolder ) ;
				}

				return e ;
			}else{
				return cs::failedToCreateMountPoint ;
			}
		} ;

		if( utility::pathExists( opt.cipherFolder + "/cryfs.config" ) ){

			return _mount( "cryfs",opt ) ;
		}

		auto encfs6 = opt.cipherFolder + "/.encfs6.xml" ;
		auto encfs5 = opt.cipherFolder + "/.encfs5" ;
		auto encfs4 = opt.cipherFolder + "/.encfs4" ;

		if( utility::atLeastOnePathExists( encfs6,encfs5,encfs4 ) ){

			return _mount( "encfs",opt ) ;
		}

		return cs::unknown ;
	} ) ;
}

Task::future< cs >& cryfsTask::encryptedFolderCreate( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		if( _create_folder( opt.cipherFolder ) ){

			if( _create_folder( opt.plainFolder ) ){

				auto e = _cmd( "cryfs",opt ) ;

				if( e == cs::success ){

					opt.openFolder( opt.plainFolder ) ;
				}else{
					_delete_folder( opt.plainFolder ) ;
					_delete_folder( opt.cipherFolder ) ;
				}

				return e ;
			}else{
				_delete_folder( opt.cipherFolder ) ;

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

		auto _dcd = []( QString path,bool set_offset ){

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

					e.append( { _dcd( cf,true ),_dcd( m,false ),_fs( fs ),_ro( k ) } ) ;
				}else{
					e.append( { _hash( m ),_dcd( m,false ),_fs( fs ),_ro( k ) } ) ;
				}
			}
		}

		return e ;
	} ) ;
}
