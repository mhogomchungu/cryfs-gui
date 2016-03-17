<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogMsg</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>cipher</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>key size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>device</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>loop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>used %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>active slots</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>
&quot;system volumes&quot; are volumes that either udev has identify them as such if udev is enabled or have an entry in &quot;/etc/fstab&quot;,&quot;/etc/crypttab&quot; or &quot;/etc/zuluCrypt/system_volumes.list&quot;.

If you prefer for a volume not to be considered a system volume,start the toolfrom root account and then go to &quot;menu-&gt;options-&gt;manage non system partitions&quot; and add the volume to the list and the volume will stop being considered as &quot;system&quot;.

Alternatively,you can add yourself to group &quot;zulucrypt&quot; and &quot;zulumount&quot; and all restrictions will go away.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>INFORMATION</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insufficient privilege to access a system device,
only root user or members of group zulucrypt can do that</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insufficient privilege to access a system device in read/write mode,
only root user or members of group zulucrypt-write can do that</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You do not seem to have proper permissions to access the encrypted file in %1 mode,check file permissions and try again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>
options:
	-d   path to where a volume to be auto unlocked/mounted is located
	-m   tool to use to open a default file manager(default tool is xdg-open)
	-e   start the application without showing the GUI
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>about cryfs-gui</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>hmac plugin.

This plugin generates a key using below formular:

key = hmac(sha256,passphrase,keyfile contents)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>keykeyfile plugin.

This plugin generates a key using below formular:

key = passphrase + keyfile contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>gpg plugin.

This plugin retrives a key locked in a gpg file with a symmetric key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not find &quot;gpg&quot; executable in &quot;/usr/local/bin&quot;,&quot;/usr/bin&quot; and &quot;/usr/sbin&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>INFO</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Translation will be done the next time you restart.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>checkForUpdates</name>
    <message>
        <source>ERROR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed To Check For Update.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>
Installed Version Is : %1.
Latest Version Is : %2.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Update Available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Version Info</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>cryfsGUI</name>
    <message>
        <source>cryfs-gui</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount Point Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Label</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%Used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Create Cryfs Volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount Encf&amp;s/Cryfs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Refresh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto Open Mount Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check For Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key Storage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Internal Wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change PassWord</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>KDE Wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gnome Wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Private Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Shared Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close Menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open mount point because &quot;%1&quot; tool does not appear to be working correctly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Permission to access the volume was denied
or
the volume is not supported
(LVM/MDRAID signatures found)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select An Encfs/Cryfs Volume Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to unmount %1 volume</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>favorites</name>
    <message>
        <source>Manage Favorites</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>open partition dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>open file dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove Selected Entry</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Encrypted folder address field is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount point path field is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Path To An Encrypted Folder</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>keyDialog</name>
    <message>
        <source>unlock and mount a luks volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mount In &amp;Read Only Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Share Mount Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;ptions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Create</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create A New Cryfs Volume</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlocking &quot;%1&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set File System Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>KeyFile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key+KeyFile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select A Folder To Create A Mount Point In</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select A File To Be Used As A Keyfile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume Name Field Is Empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key Field Is Empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Internal wallet is not configured</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The volume does not appear to have an entry in the wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to unlock a cryfs volume.
Wrong password entered</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to unlock an encfs volume.
Wrong password entered</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to create mount point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to unlock the volume.
Not supported volume encountered</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to complete the task.
Backend not responding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to complete the task.
An unknown error has occured</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyfile path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to complete the request.
cryfs executable could not be found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to complete the request.
encfs executable could not be found</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>oneinstance</name>
    <message>
        <source>Previous instance seem to have crashed,trying to clean up before starting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There seem to be another instance running,exiting this one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>plugin</name>
    <message>
        <source>Key Generator Using A Passphrase And A KeyFile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Set Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create an encryption key that is made up of a passphrase and a keyfile.

A volume created with a key generated here should be opened with &quot;hmac&quot; plugin.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Passphrase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>KeyFile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>KeyFile Not Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed To Generate Key</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>readOnlyWarning</name>
    <message>
        <source>WARNING</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do Not Show This Message Again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting this option will cause the volume to open in read only mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>walletconfig</name>
    <message>
        <source>Manage Volumes In A Wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do&amp;ne</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>WARNING!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Are you sure you want to delete a volume with an id of &quot;%1&quot;?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed To Add the Key In The Wallet.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>walletconfiginput</name>
    <message>
        <source>Add An Entry To Wallet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Volume ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Repeat Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ERROR!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Atleast one required field is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Passphrases do not match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select A Volume</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
