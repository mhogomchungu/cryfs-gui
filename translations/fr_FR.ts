<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fr_FR">
<context>
    <name>DialogMsg</name>
    <message>
        <location filename="../src/dialogmsg.ui" line="17"/>
        <source>Dialog</source>
        <translation>Boîte de dialogue</translation>
    </message>
    <message>
        <location filename="../src/dialogmsg.ui" line="29"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../src/dialogmsg.ui" line="42"/>
        <source>&amp;Yes</source>
        <translation>&amp;Oui</translation>
    </message>
    <message>
        <location filename="../src/dialogmsg.ui" line="55"/>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <location filename="../src/dialogmsg.ui" line="68"/>
        <source>text</source>
        <translation>texte</translation>
    </message>
</context>
<context>
    <name>LXQt::Wallet::changePassWordDialog</name>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="108"/>
        <source>Create a new wallet</source>
        <translation>Créer un nouveau trousseau</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="110"/>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="136"/>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="272"/>
        <source>Passwords do not match</source>
        <translation>Mots de passe non correspondants</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="217"/>
        <source>Wallet password could not be changed</source>
        <translation>Le mot de passe du trousseau ne peut pas être changé</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="226"/>
        <source>Wallet could not be opened with the presented key</source>
        <translation>Le trousseau ne peut pas être ouvert avec cette clé</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.cpp" line="236"/>
        <source>New passwords do not match</source>
        <translation>Nouveaux mots de passe non correspondants</translation>
    </message>
</context>
<context>
    <name>LXQt::Wallet::password_dialog</name>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.cpp" line="69"/>
        <source>Wallet could not be opened with the presented key</source>
        <translation>Le trousseau ne peut pas être ouvert avec cette clé</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/utility.cpp" line="81"/>
        <source>
options:
	-d   Path to where a volume to be auto unlocked/mounted is located.
	-m   Tool to use to open a default file manager(default tool is xdg-open).
	-e   Start the application without showing the GUI.
	-b   A name of a backend to retrieve a password from when a volume is open from CLI.
	     Supported backends are: &quot;internal&quot;,&quot;kwallet&quot; and &quot;gnomewallet.
	     The first one is always present but the rest are compile time dependencies.
	-k   When opening a volume from CLI,a value of &quot;rw&quot; will open the volume in read\write
	     mode and a value of &quot;ro&quot; will open the volume in read only mode.
	-z   Full path of the mount point to be used when the volume is opened from CLI.
	     This option is optional.
        -c   Set Cryfs Configuration File Path when a volume is opened from CLI.
	-i   Set inactivity timeout(in minutes) to dismount the volume when mounted from CLI.

</source>
        <translation>
options:
	-d   Chemin d&apos;accès où le volume sera auto déverrouillé/monté.
	-m   outil à utiliser pour ouvrir le gestionnaire de fichiers (par défaut : xdg-open).
	-e   Démarre l&apos;application sans utiliser de GUI .
	-b   le nom d&apos;un gestionnaire de mot de passe quand un volume est ouvert depuis la ligne de commande.
	     Noms de gestionnaires supportés: &quot;internal&quot;,&quot;kwallet&quot; and &quot;gnomewallet.
	     Le premier est toujours présent, les autres dépendent d&apos;installations.
	-k   Lorsque vous ouvrez en ligne de commande, une valeur comme &quot;rw&quot; ouvrira en lecture/écriture
	     alors que la valeur &quot;ro&quot; ouvrira en lecture seule.
	-z   Chemin d&apos;accès complet du point de montage à utiliser quand le volume est ouvert en ligne de commande.
	     cette option est facultative.
	-i   Paramètre le temps d&apos;inactivité (en minutes) pour démonter le volume lorsqu&apos;il a été monté en ligne de commande.

</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="406"/>
        <source>Manage Favorites</source>
        <translation>Gérer les Favoris</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="407"/>
        <source>Mount All</source>
        <translation>Tout Monter</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="434"/>
        <source>about cryfs-gui</source>
        <translation>A propos de cryfs-gui</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="624"/>
        <source>hmac plugin.

This plugin generates a key using below formular:

key = hmac(sha256,passphrase,keyfile contents)</source>
        <translation>hmac plugin.

Ce plugin génère une clé en utilisant la formule ci-dessous:

key = hmac(sha256,passphrase,keyfile contents)</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="629"/>
        <source>keykeyfile plugin.

This plugin generates a key using below formular:

key = passphrase + keyfile contents</source>
        <translation>keykeyfile plugin.

Ce plugin génère une clé à l&apos;aide de la formule suivante:

key = passphrase + keyfile contents
</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="634"/>
        <source>gpg plugin.

This plugin retrives a key locked in a gpg file with a symmetric key</source>
        <translation>gpg plugin.

Ce plugin récupère une clé de verrouillage dans un fichier gpg avec une clé symétrique</translation>
    </message>
    <message>
        <location filename="../src/checkforupdates.cpp" line="48"/>
        <location filename="../src/checkforupdates.cpp" line="88"/>
        <location filename="../src/utility.cpp" line="652"/>
        <source>ERROR</source>
        <translation>ERREUR</translation>
    </message>
    <message>
        <location filename="../src/checkforupdates.cpp" line="33"/>
        <source>%1&quot;%2&quot; Installed Version Is : %3.
Latest Version Is : %4.</source>
        <translation>%1&quot;%2&quot; version installée : %3.
Dernière version disponible : %4.</translation>
    </message>
    <message>
        <location filename="../src/checkforupdates.cpp" line="47"/>
        <location filename="../src/checkforupdates.cpp" line="86"/>
        <source>Failed To Check For Updates.</source>
        <translation>Impossible de tester si la version est à jour.</translation>
    </message>
    <message>
        <location filename="../src/checkforupdates.cpp" line="72"/>
        <source>Update Available</source>
        <translation>Mise à jour disponible</translation>
    </message>
    <message>
        <location filename="../src/checkforupdates.cpp" line="84"/>
        <source>Version Info</source>
        <translation>Infos de Version</translation>
    </message>
    <message>
        <location filename="../src/utility.cpp" line="652"/>
        <source>Could not find &quot;gpg&quot; executable in &quot;/usr/local/bin&quot;,&quot;/usr/bin&quot; and &quot;/usr/sbin&quot;</source>
        <translation>Ne trouve pas de &quot;gpg&quot; executable dans &quot;/usr/local/bin&quot;,&quot;/usr/bin&quot; ou &quot;/usr/sbin&quot;</translation>
    </message>
</context>
<context>
    <name>changePassWordDialog</name>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="14"/>
        <source>Change Wallet&apos;s Password</source>
        <translation>Changer le mot de passe du trousseau</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="42"/>
        <source>C&amp;hange</source>
        <translation>C&amp;hanger</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="55"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="126"/>
        <source>Enter Current Password Below.</source>
        <translation>Entrer le mot de passe actuel ci dessous.</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="145"/>
        <source>Enter New Password Below.</source>
        <translation>Entrer le nouveau mot de passe ci dessous.</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="164"/>
        <source>Re Enter New Password Below.</source>
        <translation>Ré-Entrer le nouveau mot de passe ci dessous.</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="183"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="196"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;An application &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%1&lt;/span&gt;&apos; has made a request for a password of its wallet &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%2&lt;/span&gt;&apos; to be changed&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;br /&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;L&apos;application &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%1&lt;/span&gt;&apos; a demandé de modifier le mot de passe de son trousseau   &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%2&lt;/span&gt;&apos;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;br /&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/changepassworddialog.ui" line="214"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;Enter password information below to create a new wallet &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%1&lt;/span&gt;&apos; for application &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%2&lt;/span&gt;&apos;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;Saisissez le mot de passe du trousseau ci-après pour créer un nouveau trousseau &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%1&lt;/span&gt;&apos; pour l&apos;application &apos;&lt;span style=&quot; font-weight:600;&quot;&gt;%2&lt;/span&gt;&apos;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>cryfsGUI</name>
    <message>
        <location filename="../src/cryfs-gui.ui" line="17"/>
        <source>cryfs-gui</source>
        <translation>cryfs-gui</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="55"/>
        <source>Volume Path</source>
        <translation>Chemin du volume</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="63"/>
        <source>Mount Point Path</source>
        <translation>Chemin du point de montage</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="71"/>
        <source>File System</source>
        <translation>Système de Fichiers</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="79"/>
        <source>Mode</source>
        <translation>Mode</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="95"/>
        <source>&amp;Create Cryfs Volume</source>
        <translation>&amp;Créer un Volume Cryfs</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="108"/>
        <source>Mount Encf&amp;s/Cryfs</source>
        <translation>Monter Encf&amp;s/Cryfs</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="115"/>
        <source>&amp;Refresh</source>
        <translation>&amp;Rafraîchir</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.ui" line="122"/>
        <source>&amp;Menu</source>
        <translation>&amp;Menu</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="217"/>
        <source>Auto Open Mount Point</source>
        <translation>Ouverture automatique du point de montage</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="220"/>
        <source>Reuse Mount Point</source>
        <translation>Réutiliser le Point de Montage</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="226"/>
        <source>Unmount All</source>
        <translation>Démonter tout</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="230"/>
        <location filename="../src/cryfs-gui.cpp" line="255"/>
        <location filename="../src/cryfs-gui.cpp" line="324"/>
        <source>Internal Wallet</source>
        <translation>Trousseau interne</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="232"/>
        <source>Change Password</source>
        <translation>Changer le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="242"/>
        <source>Key Storage</source>
        <translation>Clé de stockage</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="256"/>
        <location filename="../src/cryfs-gui.cpp" line="328"/>
        <source>KDE Wallet</source>
        <translation>Trousseau KDE</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="257"/>
        <location filename="../src/cryfs-gui.cpp" line="332"/>
        <source>Gnome Wallet</source>
        <translation>Trousseau Gnome</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="262"/>
        <source>Favorites</source>
        <translation>Favoris</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="266"/>
        <source>Select Language</source>
        <translation>Sélectionner la langue</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="223"/>
        <source>Autocheck For Updates</source>
        <translation>Mise à jour automatique du logiciel</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="269"/>
        <source>Check For Updates</source>
        <translation>Tester si la version du logiciel est à jour</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="272"/>
        <source>About</source>
        <translation>À propos</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="274"/>
        <source>Quit</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="383"/>
        <source>Manage Favorites</source>
        <translation>Gérer les favoris</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="387"/>
        <source>Mount All</source>
        <translation>Monter tout</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="530"/>
        <source>ERROR: Volume Path Not Given.</source>
        <translation>ERREUR: Chemin du volume non fourni.</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="562"/>
        <source>ERROR: Key Not Found In The Backend.</source>
        <translation>ERREUR: Clé non trouvée dans le Gestionnaire.</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="585"/>
        <source>ERROR: Failed To Unlock Requested Volume.</source>
        <translation>ERREUR: Echec du déverrouillage du volume choisi.</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="590"/>
        <source>ERROR: Failed To Unlock Requested Backend.</source>
        <translation>ERREUR: Echec du déverrouillage du Gestionnaire sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="624"/>
        <source>INFORMATION</source>
        <translation>INFORMATION</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="620"/>
        <source>Failed To Read Volume Properties</source>
        <translation>Echec de la lecture des propriétés du volume</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="663"/>
        <source>Block Size: %1</source>
        <translation>Taille des blocs: %1</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="664"/>
        <source>Used Blocks: %2</source>
        <translation>Blocs utilisés: %2</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="665"/>
        <source>Free Blocks: %3</source>
        <translation>Blocs Libres: %3</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="741"/>
        <source>Unmount</source>
        <translation>Démonter</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="743"/>
        <source>Open Folder</source>
        <translation>Ouvrir Répertoire</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="745"/>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="761"/>
        <source>Close Menu</source>
        <translation>Fermer le Menu</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="813"/>
        <source>WARNING</source>
        <translation>Attention</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="814"/>
        <source>Could Not Open Mount Point Because &quot;%1&quot; Tool Does Not Appear To Be Working Correctly.</source>
        <translation>Impossible d&apos;ouvrir le point de montage parce que l&apos;outil &quot;%1&quot; ne semble pas fonctionner correctement.</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="620"/>
        <location filename="../src/cryfs-gui.cpp" line="926"/>
        <location filename="../src/cryfs-gui.cpp" line="1027"/>
        <source>ERROR</source>
        <translation>ERREUR</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="666"/>
        <source>Total Blocks %4</source>
        <translation>Total des Blocs %4</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="667"/>
        <source>Used Space: %5</source>
        <translation>Espace Utilisé: %5</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="668"/>
        <source>Free Space: %6</source>
        <translation>Espace Libre: %6</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="669"/>
        <source>Total Space: %7</source>
        <translation>Espace Total: %7</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="670"/>
        <source>Used %: %8</source>
        <translation>Utilisé %: %8</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="927"/>
        <source>Permission To Access The Volume Was Denied
Or
The Volume Is Not Supported</source>
        <translation>La permission d&apos;accéder au volume a été refusée
ou
le volume n&apos;est pas supporté</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="947"/>
        <source>Select An Encfs/Cryfs Volume Directory</source>
        <translation>Sélectionner un répertoire de volume Encfs/Cryfs</translation>
    </message>
    <message>
        <location filename="../src/cryfs-gui.cpp" line="1027"/>
        <source>Failed To Unmount %1 Volume</source>
        <translation>Impossible de démonter le Volume %1</translation>
    </message>
</context>
<context>
    <name>favorites</name>
    <message>
        <location filename="../src/favorites.ui" line="17"/>
        <source>Manage Favorites</source>
        <translation>Gérer les Favoris</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="29"/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="64"/>
        <source>Volume ID</source>
        <translation>Volume ID</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="72"/>
        <source>Mount Point</source>
        <translation>Point de montage</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="89"/>
        <source>Encrypted Folder Path</source>
        <translation>Chemin d&apos;accès du répertoire chiffré</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="108"/>
        <source>Mount Point Path</source>
        <translation>Ouvrir Boite de Dialogue de fichier</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="147"/>
        <source>open file dialog</source>
        <translation>Ouvrir Boite de Dialogue de fichier</translation>
    </message>
    <message>
        <location filename="../src/favorites.ui" line="163"/>
        <source>&amp;Done</source>
        <translation>&amp;Fini</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="153"/>
        <source>Remove Selected Entry</source>
        <translation>Enlever l&apos;entrée sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="157"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="208"/>
        <location filename="../src/favorites.cpp" line="212"/>
        <source>ERROR!</source>
        <translation>ERREUR!</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="208"/>
        <source>Encrypted folder address field is empty</source>
        <translation>Champ d&apos;adresse du répertoire chiffré vide</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="212"/>
        <source>Mount point path field is empty</source>
        <translation>Le champ Chemin du point de montage est vide</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="229"/>
        <source>Path To An Encrypted Folder</source>
        <translation>Chemin d&apos;accès au répertoire chiffré</translation>
    </message>
    <message>
        <location filename="../src/favorites.cpp" line="239"/>
        <source>Path To Mount Folder</source>
        <translation>Chemin d&apos;accès au répertoire de montage</translation>
    </message>
</context>
<context>
    <name>keyDialog</name>
    <message>
        <location filename="../src/keydialog.ui" line="29"/>
        <source>&amp;Open</source>
        <translation>&amp;Ouvrir</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="45"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="58"/>
        <location filename="../src/keydialog.cpp" line="182"/>
        <location filename="../src/keydialog.cpp" line="742"/>
        <source>Key</source>
        <translation>Clé</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="100"/>
        <location filename="../src/keydialog.cpp" line="66"/>
        <source>Volume Name</source>
        <translation>Nom du montage</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="142"/>
        <source>Mount In &amp;Read Only Mode</source>
        <translation>Monter en &amp;Lecture Seule</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="155"/>
        <source>&amp;Share Mount Point</source>
        <translation>&amp;Share (Partage) Point de montage</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="168"/>
        <source>O&amp;ptions</source>
        <translation>O&amp;ptions</translation>
    </message>
    <message>
        <location filename="../src/keydialog.ui" line="201"/>
        <source>Volume Path</source>
        <translation>Chemin du volume</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="64"/>
        <source>&amp;Create</source>
        <translation>&amp;Créer</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="85"/>
        <location filename="../src/keydialog.cpp" line="234"/>
        <source>Create A New Cryfs Volume</source>
        <translation>Créer un nouveau volume Cryfs</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="87"/>
        <source>Unlocking &quot;%1&quot;</source>
        <translation>Déchiffrer &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="91"/>
        <source>Mount Path</source>
        <translation>Chemin de montage</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="183"/>
        <source>KeyFile</source>
        <translation>Fichier de clé</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="184"/>
        <source>Key+KeyFile</source>
        <translation>Clé+fichier de clé</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="185"/>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="208"/>
        <source>Set Idle Timeout</source>
        <translation>Paramétrer le délai d&apos;inactivité</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="211"/>
        <source>Set Cryfs Configuration File Path</source>
        <translation>Définir le chemin du fichier de configuration Cryfs</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="225"/>
        <source>Passphrase Quality: 0%</source>
        <translation>Force de la Passphrase : 0%</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="227"/>
        <source>Passphrase Quality: %1%</source>
        <translation>Force de la Passphrase : %1%</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="232"/>
        <source>Passphrase Quality: 100%</source>
        <translation>Force de la Passphrase : 100%</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="245"/>
        <source>Automatically Unmount After Specified Minutes of Inactivity Is Reached.</source>
        <translation>Démonter automatiquement lorsque que le délai d&apos;inactivité spécifié -en minutes- est atteint.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="255"/>
        <source>Unlock A Cryfs Volume With Specified Configuration File.</source>
        <translation>Déverrouiller un volume Cryfs avec le fichier de configuration proposé.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="304"/>
        <location filename="../src/keydialog.cpp" line="317"/>
        <source>Select A Folder To Create A Mount Point In.</source>
        <translation>Sélectionner un répertoire pour y créer un point de montage.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="373"/>
        <source>Select A File To Be Used As A Keyfile.</source>
        <translation>Sélectionner un fichier afin de l&apos;utiliser comme un fichier de clé.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="405"/>
        <location filename="../src/keydialog.cpp" line="416"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="436"/>
        <location filename="../src/keydialog.cpp" line="443"/>
        <location filename="../src/keydialog.cpp" line="485"/>
        <location filename="../src/keydialog.cpp" line="518"/>
        <location filename="../src/keydialog.cpp" line="523"/>
        <location filename="../src/keydialog.cpp" line="528"/>
        <location filename="../src/keydialog.cpp" line="533"/>
        <location filename="../src/keydialog.cpp" line="538"/>
        <location filename="../src/keydialog.cpp" line="543"/>
        <location filename="../src/keydialog.cpp" line="548"/>
        <location filename="../src/keydialog.cpp" line="551"/>
        <location filename="../src/keydialog.cpp" line="568"/>
        <location filename="../src/keydialog.cpp" line="579"/>
        <location filename="../src/keydialog.cpp" line="588"/>
        <location filename="../src/keydialog.cpp" line="620"/>
        <location filename="../src/keydialog.cpp" line="629"/>
        <location filename="../src/keydialog.cpp" line="638"/>
        <source>ERROR</source>
        <translation>ERREUR</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="436"/>
        <source>Volume Name Field Is Empty.</source>
        <translation>Le champ du Nom du Volume est vide.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="443"/>
        <source>Key Field Is Empty.</source>
        <translation>Champ de la clé est vide.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="466"/>
        <source>ERROR!</source>
        <translation>ERREUR!</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="466"/>
        <source>Internal Wallet Is Not Configured.</source>
        <translation>Trousseau Interne non configuré.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="485"/>
        <source>The Volume Does Not Appear To Have An Entry In The Wallet.</source>
        <translation>Le volume ne semble pas avoir une entrée dans le trousseau.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="518"/>
        <source>Failed To Unlock A Cryfs Volume.
Wrong Password Entered.</source>
        <translation>Echec de l&apos;ouverture du volume Cryfs
Mot de passe entré incorrect.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="523"/>
        <source>Failed To Unlock An Encfs Volume.
Wrong Password Entered.</source>
        <translation>Echec de l&apos;ouverture du volume Encfs
Mot de passe entré incorrect.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="528"/>
        <source>Failed To Complete The Request.
Cryfs Executable Could Not Be Found.</source>
        <translation>Impossible de finir la requête.
L&apos;exécutable Cryfs est introuvable.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="533"/>
        <source>Failed To Complete The Request.
Encfs Executable Could Not Be Found.</source>
        <translation>Impossible de finir la requête.
L&apos;exécutable Encfs est introuvable.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="538"/>
        <source>Failed To Create Mount Point.</source>
        <translation>Echec de la création du point de montage.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="543"/>
        <source>Failed To Unlock The Volume.
Not Supported Volume Encountered.</source>
        <translation>Echec de l&apos;ouverture du volume.
Volume non supporté rencontré.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="548"/>
        <source>Failed To Complete The Task.
Backend Not Responding.</source>
        <translation>Impossible de finir la tâche.
Le gestionnaire de trousseau ne répond pas.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="551"/>
        <source>Failed To Complete The Task.
An Unknown Error Has Occured.</source>
        <translation>Impossible de finir la tâche.
Une erreur inconnue est survenue.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="568"/>
        <source>Encrypted Folder Path Is Already Taken.</source>
        <translation>Chemin d&apos;accès du répertoire chiffré déjà occupé.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="579"/>
        <location filename="../src/keydialog.cpp" line="620"/>
        <source>Mount Point Path Already Taken.</source>
        <translation>Point de montage déjà occupé.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="588"/>
        <location filename="../src/keydialog.cpp" line="638"/>
        <source>Atleast One Required Field Is Empty.</source>
        <translation>Au moins un des champs qui est requis est vide.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="629"/>
        <source>Encrypted Folder Appear To Not Be Present.</source>
        <translation>Le chemin du répertoire chiffré ne semble pas présent.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="678"/>
        <source>WARNING</source>
        <translation>ATTENTION</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="678"/>
        <source>KeyFile Contents Will Be Trancated On The First Encountered NewLine Character.</source>
        <translation>KeyFile contenu sera Trancated sur le premier caractère NewLine Rencontrés.</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="730"/>
        <source>Plugin name</source>
        <translation>Plugin name</translation>
    </message>
    <message>
        <location filename="../src/keydialog.cpp" line="754"/>
        <source>Keyfile path</source>
        <translation>Chemin du fichier de clé</translation>
    </message>
</context>
<context>
    <name>oneinstance</name>
    <message>
        <location filename="../src/oneinstance.cpp" line="108"/>
        <source>Previous instance seem to have crashed,trying to clean up before starting</source>
        <translation>L&apos;instance précédente semble avoir crashé, tentative de réinitialisation avant démarrage</translation>
    </message>
    <message>
        <location filename="../src/oneinstance.cpp" line="115"/>
        <source>There seem to be another instance running,exiting this one</source>
        <translation>Il semble y avoir une autre instance en cours d&apos;exécution, sortie de celle-ci</translation>
    </message>
</context>
<context>
    <name>options</name>
    <message>
        <location filename="../src/options.ui" line="17"/>
        <source>Set Mount Options</source>
        <translation>Paramétrer les options de montage</translation>
    </message>
    <message>
        <location filename="../src/options.ui" line="38"/>
        <source>&amp;Set</source>
        <translation>&amp;Paramétrer</translation>
    </message>
    <message>
        <location filename="../src/options.ui" line="51"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/options.cpp" line="65"/>
        <source>Select Cryfs Configuration File</source>
        <translation>Sélectionner un fichier de configuration Cryfs</translation>
    </message>
    <message>
        <location filename="../src/options.cpp" line="113"/>
        <source>ERROR</source>
        <translation>ERREUR</translation>
    </message>
    <message>
        <location filename="../src/options.cpp" line="113"/>
        <source>Option Field Is Empty.</source>
        <translation>Champ d&apos;options vide.</translation>
    </message>
</context>
<context>
    <name>password_dialog</name>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="14"/>
        <source>lxqt wallet service</source>
        <translation>lxqt service de trousseau</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="42"/>
        <source>The application &apos;%1&apos; has requested to open the wallet &apos;%2&apos;.Enter the password below for this wallet to unlock it</source>
        <translation>L&apos;application &apos;%1&apos; a demandé d&apos;ouvrir le trousseau &apos;%2&apos;. Enter le mot de passe de ce trousseau ci-dessous pour le déverrouiller</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="74"/>
        <source>Wallet &apos;%1&apos; does not exist,do you want to create it?</source>
        <translation>Le trousseau &apos;%1&apos; n&apos;existe pas, voulez vous le créer ?</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="90"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;Wallet &lt;span style=&quot; font-weight:600;&quot;&gt;&apos;%1&apos;&lt;/span&gt; does not exist, do you want to create it?&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;Le trousseau &lt;span style=&quot; font-weight:600;&quot;&gt;&apos;%1&apos;&lt;/span&gt; n&apos;existe pas, voulez-vous en créer un ?&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="107"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;An application &lt;span style=&quot; font-weight:600;&quot;&gt;&apos;%1&apos;&lt;/span&gt; has requested to open a wallet&lt;span style=&quot; font-weight:600;&quot;&gt; &apos;%2&apos;&lt;/span&gt;. Enter the password below for this wallet to unlock it.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Sans Serif&apos;; font-size:9pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;L&apos;application &lt;span style=&quot; font-weight:600;&quot;&gt;&apos;%1&apos;&lt;/span&gt; a demandé d&apos;ouvrir le trousseau&lt;span style=&quot; font-weight:600;&quot;&gt; &apos;%2&apos;&lt;/span&gt;.Saisissez le mot de passe du trousseau ci-après pour le déverrouiller.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="124"/>
        <source>&amp;Unlock</source>
        <translation>&amp;Déverrouiller</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="141"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="154"/>
        <source>ok</source>
        <translation>ok</translation>
    </message>
    <message>
        <location filename="../src/lxqt_wallet/frontend/password_dialog.ui" line="167"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
</context>
<context>
    <name>plugin</name>
    <message>
        <location filename="../src/plugin.ui" line="17"/>
        <source>Key Generator Using A Passphrase And A KeyFile</source>
        <translation>Générateur de clé utilisant une passphrase et un fichier de clé</translation>
    </message>
    <message>
        <location filename="../src/plugin.ui" line="29"/>
        <source>&amp;Set Key</source>
        <translation>&amp;Paramétrer une clé</translation>
    </message>
    <message>
        <location filename="../src/plugin.ui" line="42"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/plugin.ui" line="93"/>
        <source>Create an encryption key that is made up of a passphrase and a keyfile.

A volume created with a key generated here should be opened with &quot;hmac&quot; plugin.</source>
        <translation>Créer une clé de chiffrement qui est composée d&apos;une phrase et d&apos;un keyfile.

Un volume créé avec une clé générée ici devra être ouvert avec le plugin &quot;hmac&quot;.</translation>
    </message>
    <message>
        <location filename="../src/plugin.ui" line="115"/>
        <source>Passphrase</source>
        <translation>Passphrase</translation>
    </message>
    <message>
        <location filename="../src/plugin.ui" line="144"/>
        <location filename="../src/plugin.cpp" line="156"/>
        <source>KeyFile</source>
        <translation>Fichier de clé</translation>
    </message>
    <message>
        <location filename="../src/plugin.cpp" line="102"/>
        <location filename="../src/plugin.cpp" line="145"/>
        <source>ERROR</source>
        <translation>ERREUR</translation>
    </message>
    <message>
        <location filename="../src/plugin.cpp" line="102"/>
        <source>KeyFile Not Set</source>
        <translation>Fichier de clé non actif</translation>
    </message>
    <message>
        <location filename="../src/plugin.cpp" line="145"/>
        <source>Failed To Generate Key</source>
        <translation>Echec de la génération de clé</translation>
    </message>
</context>
<context>
    <name>readOnlyWarning</name>
    <message>
        <location filename="../src/readonlywarning.ui" line="14"/>
        <source>WARNING</source>
        <translation>ATTENTION</translation>
    </message>
    <message>
        <location filename="../src/readonlywarning.ui" line="26"/>
        <source>Do Not Show This Message Again.</source>
        <translation>Ne plus montrer ce message désormais.</translation>
    </message>
    <message>
        <location filename="../src/readonlywarning.ui" line="39"/>
        <source>Setting This Option Will Cause The Volume To Open In Read Only Mode.</source>
        <translation>Définir cette option ouvrira le volume en mode lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/readonlywarning.ui" line="58"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
</context>
<context>
    <name>walletconfig</name>
    <message>
        <location filename="../src/walletconfig.ui" line="17"/>
        <source>Manage Volumes In A Wallet</source>
        <translation>Gérer les volumes dans un trousseau</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.ui" line="32"/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.ui" line="51"/>
        <source>&amp;Delete</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.ui" line="64"/>
        <source>Do&amp;ne</source>
        <translation>&amp;Fini</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.ui" line="111"/>
        <source>Volume ID</source>
        <translation>Volume ID</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.ui" line="116"/>
        <source>Comment</source>
        <translation>Commentaires</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.cpp" line="81"/>
        <source>WARNING!</source>
        <translation>ATTENTION!</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.cpp" line="81"/>
        <source>Are you sure you want to delete a volume with an id of &quot;%1&quot;?</source>
        <translation>Êtes vous sûr d&apos;effacer un volume avec un ID de &quot;%1&quot;?</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.cpp" line="160"/>
        <source>ERROR!</source>
        <translation>ERREUR!</translation>
    </message>
    <message>
        <location filename="../src/walletconfig.cpp" line="160"/>
        <source>Failed To Add the Key In The Wallet.</source>
        <translation>Echec de l&apos;ajout de la clé dans le trousseau.</translation>
    </message>
</context>
<context>
    <name>walletconfiginput</name>
    <message>
        <location filename="../src/walletconfiginput.ui" line="14"/>
        <source>Add An Entry To Wallet</source>
        <translation>Ajouter une entrée au Trousseau</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="26"/>
        <source>&amp;Add</source>
        <translation>&amp;Ajouter</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="39"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="61"/>
        <source>Volume ID</source>
        <translation>Volume ID</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="80"/>
        <source>Comment</source>
        <translation>Commentaires</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="99"/>
        <source>Key</source>
        <translation>Clé</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.ui" line="118"/>
        <source>Repeat Key</source>
        <translation>Réentrer la clé</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.cpp" line="95"/>
        <location filename="../src/walletconfiginput.cpp" line="104"/>
        <source>ERROR!</source>
        <translation>ERREUR!</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.cpp" line="95"/>
        <source>Atleast one required field is empty</source>
        <translation>Au moins un des champs qui est requis est vide</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.cpp" line="104"/>
        <source>Passphrases do not match</source>
        <translation>Passphrases non correspondantes</translation>
    </message>
    <message>
        <location filename="../src/walletconfiginput.cpp" line="127"/>
        <source>Select A Volume</source>
        <translation>Sélectionner un volume</translation>
    </message>
</context>
</TS>
