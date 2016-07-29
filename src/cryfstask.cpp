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

#include <iostream>

using cs = cryfsTask::status ;

static bool _create_folder( const QString& m )
{
	if( utility::pathExists( m ) ){

		return utility::reUseMountPoint() ;
	}else{
		QDir e ;
		return e.mkpath( m ) ;
	}
}

static QString _makePath( const QString& e )
{
	return utility::Task::makePath( e ) ;
}

bool cryfsTask::deleteMountFolder( const QString& m )
{
	if( utility::reUseMountPoint() ){

		return false ;
	}else{
		QDir e ;
		return e.rmdir( m ) ;
	}
}

Task::future< bool >& cryfsTask::encryptedFolderUnMount( const QString& m )
{
	return Task::run< bool >( [ m ](){

		auto cmd = "fusermount -u " + _makePath( m ) ;

		utility::Task::waitForOneSecond() ;

		for( int i = 0 ; i < 5 ; i++ ){

			if( utility::Task( cmd,10000 ).success() ){

				return true ;
			}else{
				utility::Task::waitForOneSecond() ;
			}
		}

		return false ;
	} ) ;
}

static cs _cmd( const QString& app,const cryfsTask::options& opt,const QString& configFilePath )
{
        auto _args = []( const QString& exe,const cryfsTask::options& opt,
                        const QString& type,const QString& configFilePath ){

		auto cipherFolder = _makePath( opt.cipherFolder ) ;

		auto mountPoint   = _makePath( opt.plainFolder ) ;

		auto mountOptions = [ & ](){

			if( !opt.mOpt.isEmpty() ){

				if( type == "cryfs" ){

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

                auto configPath = [ & ](){

                        if( type == "cryfs" && !configFilePath.isEmpty() ){

				return "--config " + _makePath( configFilePath ) ;
                        }else{
                                return QString() ;
                        }
                }() ;

		QString e = "%1 %2 %3 %4 %5 %6 -o fsname=%7@%8 -o subtype=%9" ;

		auto opts = e.arg( exe,cipherFolder,mountPoint,mountOptions,configPath,
				   separator,type,cipherFolder,type ) ;

		if( opt.ro ){

			return opts + " -o ro" ;
		}else{
			return opts + " -o rw" ;
		}
	} ;

	auto exe = utility::executableFullPath( app ) ;

	if( exe.isEmpty() ){

		if( app == "cryfs" ){

			return cs::cryfsNotFound ;
		}else{
			return cs::encfsNotFound ;
		}
	}else{
		auto e = utility::Task( _args( exe,opt,app,configFilePath ),20000,[](){

			QProcessEnvironment env ;

			env.insert( "CRYFS_NO_UPDATE_CHECK","TRUE" ) ;
			env.insert( "CRYFS_FRONTEND","noninteractive" ) ;

			return env ;

		}(),opt.key.toLatin1() ) ;

		auto _printOutput = [ & ](){

			std::cout << e.output().constData() ;
			std::cout << "-------------------------------------------------------\n" ;
			std::cout << e.stdError().constData() << std::endl ;
		} ;

		if( e.finished() ){

			if( e.success() ){

				return cs::success ;
			}else{
				auto q = "Could not load config file. Did you enter the correct password?" ;
				auto p = "Error decoding volume key, password incorrect" ;

				if( e.stdError().contains( q ) || e.output().contains( p ) ){

					_printOutput() ;

					if( app == "cryfs" ){

						return cs::cryfs ;
					}else{
						return cs::encfs ;
					}
				}
			}
		}

		_printOutput() ;

		return cs::backendFail ;
	}
}

Task::future< cs >& cryfsTask::encryptedFolderMount( const options& opt )
{
	return Task::run< cs >( [ opt ](){

                auto _mount = []( const QString& app,const options& opt,const QString& configFilePath ){

			if( _create_folder( opt.plainFolder ) ){

                                auto e = _cmd( app,opt,configFilePath ) ;

				if( e == cs::success ){

					opt.openFolder( opt.plainFolder ) ;
				}else{
					cryfsTask::deleteMountFolder( opt.plainFolder ) ;
				}

				return e ;
			}else{
				return cs::failedToCreateMountPoint ;
			}
		} ;

                if( opt.configFilePath.isEmpty() ){

                        if( utility::pathExists( opt.cipherFolder + "/cryfs.config" ) ){

                                return _mount( "cryfs",opt,"" ) ;
                        }else{
                                auto encfs6 = opt.cipherFolder + "/.encfs6.xml" ;
                                auto encfs5 = opt.cipherFolder + "/.encfs5" ;
                                auto encfs4 = opt.cipherFolder + "/.encfs4" ;

                                if( utility::atLeastOnePathExists( encfs6,encfs5,encfs4 ) ){

                                        return _mount( "encfs",opt,"" ) ;
                                }else{
                                        return cs::unknown ;
                                }
                        }
                }else{
			if( utility::pathExists( opt.configFilePath ) ){

				return _mount( "cryfs",opt,opt.configFilePath ) ;
			}else{
				return cs::unknown ;
			}
                }
	} ) ;
}

Task::future< cs >& cryfsTask::encryptedFolderCreate( const options& opt )
{
	return Task::run< cs >( [ opt ](){

		if( _create_folder( opt.cipherFolder ) ){

			if( _create_folder( opt.plainFolder ) ){

				auto e = _cmd( "cryfs",opt,[ & ](){

					if( opt.configFilePath.startsWith( "/" ) ||
							opt.configFilePath.isEmpty() ){

						return opt.configFilePath ;
					}else{
						return utility::homePath() + "/" + opt.configFilePath ;
					}
				}() ) ;

				if( e == cs::success ){

					opt.openFolder( opt.plainFolder ) ;
				}else{
					cryfsTask::deleteMountFolder( opt.plainFolder ) ;
					cryfsTask::deleteMountFolder( opt.cipherFolder ) ;
				}

				return e ;
			}else{
				cryfsTask::deleteMountFolder( opt.cipherFolder ) ;

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

			/*
			 * jenkins one at a time hash function.
			 *
			 * https://en.wikipedia.org/wiki/Jenkins_hash_function
			 */

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

				return path.mid( 6 ) ;
			}else{
				return path ;
			}
		} ;

		auto _fs = []( const QString& e ){

			return e.mid( 5 ) ;
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
