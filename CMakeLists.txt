cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0017 OLD )
set_property( GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS ON )

project( cryfs-gui )

add_definitions( -D_FILE_OFFSET_BITS=64 -Wextra -Wall -pedantic -I${PROJECT_BINARY_DIR}/cryfs-gui/ -I${PROJECT_BINARY_DIR})
include_directories( ${PROJECT_BINARY_DIR}/cryfs-gui/ )

INCLUDE( GNUInstallDirs )

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY )

add_custom_target( uninstall
COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )

INCLUDE(CMakeDependentOption)
INCLUDE(FindPkgConfig)

add_definitions( -D_DEFAULT_SOURCE -fstack-protector-all --param ssp-buffer-size=4 )
include_directories( ${PROJECT_BINARY_DIR} )

if( LIB_SUFFIX )
	set( CMAKE_INSTALL_LIBDIR "${LIB_SUFFIX}" )
	set( CMAKE_INSTALL_FULL_LIBDIR "${CMAKE_INSTALL_PREFIX}/${LIB_SUFFIX}" )
endif()

file( WRITE ${PROJECT_BINARY_DIR}/locale_path.h "\n#define TRANSLATION_PATH \"${CMAKE_INSTALL_FULL_DATADIR}/cryfs-gui/translations/\"\n")

file( WRITE  ${PROJECT_BINARY_DIR}/install_prefix.h "\n#define INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}/\"\n" )

set( PGR_VERSION "1.1.0" )

file( WRITE ${PROJECT_BINARY_DIR}/version.h "
#ifndef THIS_VERSION
#define THIS_VERSION \"${PGR_VERSION}\"
#endif
\n
#ifndef CRYFS_GUI_VERSION
#define CRYFS_GUI_VERSION
#define VERSION_STRING \"Version  : ${PGR_VERSION}\\nCopyright: 2016 Francis Banyikwa,mhogomchungu@gmail.com\\nLicense  : GPLv2+\"
#endif
\n" )

if( REUSEMOUNTPOINT )
	file( WRITE  ${PROJECT_BINARY_DIR}/reuse_mount_point.h "\n#define REUSE_MOUNT_POINT 1\n" )
else()
	file( WRITE  ${PROJECT_BINARY_DIR}/reuse_mount_point.h "\n#define REUSE_MOUNT_POINT 0\n" )
endif()

ADD_SUBDIRECTORY( src/lxqt_wallet )

add_definitions( -D_FILE_OFFSET_BITS=64 -Wextra -Wall -pedantic -std=c++11 -I${PROJECT_BINARY_DIR}/cryfs-gui/ -I${PROJECT_BINARY_DIR})

set( UI_FILES 	src/cryfs-gui.ui
		src/keydialog.ui
		src/dialogmsg.ui
		src/plugin.ui
		src/favorites.ui
		src/readonlywarning.ui
		src/walletconfig.ui
		src/walletconfiginput.ui
)

set( MOC_FILES 	src/cryfs-gui.h
		src/keydialog.h
		src/oneinstance.h
		src/monitor_mountinfo.h
		src/dialogmsg.h
		src/plugin.h
		src/favorites.h
		src/checkforupdates.h
		src/readonlywarning.h
		src/walletconfig.h
		src/walletconfiginput.h
		src/utility.h
 )

set( SRC 	src/main.cpp
		src/cryfs-gui.cpp
		src/keydialog.cpp
		src/cryfstask.cpp
		src/oneinstance.cpp
		src/monitor_mountinfo.cpp
		src/utility.cpp
		src/dialogmsg.cpp
		src/favorites.cpp
		src/checkforupdates.cpp
		src/plugin.cpp
		src/tablewidget.cpp
		src/readonlywarning.cpp
		src/walletconfig.cpp
		src/walletconfiginput.cpp
)

if( QT5 )
	find_package( Qt5Widgets REQUIRED )
	find_package( Qt5Core REQUIRED )
	find_package( Qt5Network REQUIRED )

	set( CMAKE_INCLUDE_CURRENT_DIR ON )
	include_directories( ${Qt5Widgets_INCLUDE_DIRS} )
	include_directories( ${Qt5Network_INCLUDE_DIRS} )

	add_definitions( ${Qt5Widgets_DEFINITIONS} )

	QT5_WRAP_UI( UI ${UI_FILES} )
	QT5_WRAP_CPP( MOC ${MOC_FILES} )
	QT5_ADD_RESOURCES( TRAY_RC_SRCS src/icon.qrc )

	INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

	add_executable( cryfs ${MOC} ${UI} ${SRC} ${TRAY_RC_SRCS} )

	TARGET_LINK_LIBRARIES( cryfs ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} lxqtwallet )

	set_target_properties( cryfs PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}" )
else()
	find_package( Qt4 REQUIRED )

	add_definitions( -I${Qt4_INCLUDE_DIR} )
	add_definitions( -I${QT_HEADERS_DIR} )

	INCLUDE( ${QT_USE_FILE} )
	INCLUDE( ${QT_USE_FILE} )

	INCLUDE( ${QT_USE_FILE} )

	QT4_WRAP_UI( UI ${UI_FILES} )
	QT4_WRAP_CPP( MOC ${MOC_FILES} )
	QT4_ADD_RESOURCES( TRAY_RC_SRCS src/icon.qrc )

	INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

	add_executable( cryfs ${MOC} ${UI} ${SRC} ${TRAY_RC_SRCS} )

	TARGET_LINK_LIBRARIES( cryfs -lQtCore -lQtGui -lQtNetwork lxqtwallet )

	set_target_properties( cryfs PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}" )
endif()

set_target_properties( cryfs PROPERTIES COMPILE_FLAGS "-Wextra -Wall -s -fPIC -pedantic" )

set_target_properties( cryfs PROPERTIES OUTPUT_NAME cryfs_gui )

install( TARGETS cryfs RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

install ( FILES ${PROJECT_BINARY_DIR}/cryfs-gui.desktop
DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications

PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install( FILES src/cryfs-gui.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps/ )
install( FILES src/cryfs-gui.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pixmaps )

install ( FILES translations/en_US.qm DESTINATION ${CMAKE_INSTALL_DATADIR}/cryfs-gui/translations )

#install ( FILES cryfs-gui.png DESTINATION share/icons/hicolor/32x32/apps )

# desktop file section
file( WRITE ${PROJECT_BINARY_DIR}/cryfs-gui.desktop

"[Desktop Entry]
Comment[en_US]=
Comment=
Exec=${CMAKE_INSTALL_FULL_BINDIR}/cryfs_gui -d %U
GenericName[en_US]=Encrypted Folder Manager
GenericName=Encrypted Folder Manager
Icon=cryfs-gui.png
MimeType=
Name[en_US]=cryfs-gui
Name=cryfs-gui
NoDisplay=false
Path=
StartupNotify=true
Terminal=false
Type=Application
X-DBUS-ServiceName=
X-DBUS-StartupType=
X-KDE-SubstituteUID=false
X-KDE-Username=
Categories=Security;Utility;Qt;X-MandrivaLinux-System-FileTools;\n")
