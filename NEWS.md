# CMaNGOS -- History of visible changes

This file is part of Everking. See [Copyright](COPYRIGHT.md) file for Copyright information

Most of our code is released under the GPL v2, a copy is shipped with the file [LICENSE](LICENSE).

Visit our project website for forums, links and more:
http://cmangos.net

Please submit bug reports at our bugtracker at:
https://github.com/cmangos/issues

See the file ChangeLog.md for details!

## Version 0.17
 * Move to CMaNGOS (http://cmangos.net - https://github.com/cmangos)
 * Add Prepared SQL-Statements
 * Updated MySQL client libs to 5.1.49.
 * Updated ACE framework to 5.8.3.
 * Updated G3DLite library to 8.0.1

## Version 0.16
 * G3DLite lib upgrade to G3DLite 8.0b4
 * OpenSSL lib upgrade to OpenSSL 1.0.0.
 * Sockets lib use dropped.
 * dotconfpp lib use dropped.
 * Implemented SOAP in MaNGOS.
 * Upgrade to client version 3.3.3a (build 11723).

## Version 0.15
 * Implement AV battleground.
 * Implement new DB-based gossip system for creatures/gameobjects.
 * Implement realmd support realms for different client versions.
 * New memory allocator for MaNGOS, based on Intel Threading Building Blocks library.
 * Upgrade to client version 3.2.2a (build 10505).

## Version 0.14
 * Implement possibility to reset client cache data from server side.
 * DB-based store battleground gameobjects/creatures.
 * Map search/loading improvements
 * Upgrade to client version 3.1.3 (build 9947).

## Version 0.13
 * OpenSSL lib upgrade to OpenSSL 0.9.8k.
 * ZThread replaced by similar ACE framework functionality. ZThread use dropped.
 * Visual Studio 2003 (VC71) support dropped.
 * Experemental support for Visual Studio 2010 Beta 1 added.
 * Sqlite sql engine support dropped.
 * Upgrade to client version 3.0.9 (build 9551).

## Version 0.12
 * Migrate from SVN to GIT.
 * Implemented arena system.
 * Start using Adaptive Communication Environment (ACE) framework v.5.6.6 in server network code.
 * Update Visual Leak Detector lib to Version 1.9g (beta).
 * Upgrade to client version 2.4.3 (build 8606).

## Version 0.11
 * Lots of improvements in the spell system.
 * Now use Mersenne Twister random number generator.
 * MySQL client libs updated to 5.0.56.
 * Upgrade to client version 2.4.2 (build 8278).

## Version 0.10
 * master looter lot mode,
 * FFA PvP zones and FFA PvP server type support,
 * Guild Banks,
 * unique equipped items support, including gems,
 * talent inspecting,
 * PostgreSql support,
 * sockets library updated to v.2.2.9 version,
 * 64-bit binaries building at Windows,
 * Upgrade to client version 2.3.0 (build 7561).

## Version 0.9
 * recipes discovery system support,
 * more auras, effects, spells, and talents working,
 * players queue at login support,
 * mangosd/realmd as Windows services support,
 * autogeneration mangosd/realmd crash reports (Windows only),
 * Visual Studio 2008 Express and Pro support,
 * division Mangos DataBase to MangosDB(WorldDB) and CharactersDB.
 * Upgrade to client version 2.2.3 (build 7359).

## Version 0.8
 * battleground support,
 * gender specific creatures,
 * game objects triggering more scripts,
 * more auras, effects, spells, and talents working,
 * dungeon specific data,
 * localization support for data,
 * vmaps for line of sight handling,
 * build support for OpenBSD and FreeBSD,
 * Upgrade to client version 2.1.3 (build 6898).

## Version 0.7, Codename "Eye of the Storm"
 * Full rewrite of the GNU autotools based build system, in order to
   smooth and clean up the build process.
 * Full rewrite of the Windows build system, allowing MaNGOS to be built
   from Visual C++ 2003 Toolkit, Visual C++ 2003, Visual C++ 2005.
 * Upgrade to client version 2.0.12 (build 6546).

## Version 0.6, Codename "Black Dragonflight"
 * MaNGOS development moved to http://sourceforge.net/projects/mangos/
 * A lots of speed improvements to data handling and data transfer have been
   completed.
 * Mail, auction house, profession, creature, and game object systems have seen
   a lot of improvements and can be considered feature complete. Some minor bugs
   still are left.
 * Rest system has been finished.
 * Initial pet stable support has been added.
 * PvP and duel system have seen lots of improvements
 * Guilds are not just a myth anymore. Coming closer to full guild support.
 * Taxi and transport system is close to a fully working system.
 * A lot more has been added.

## Version 0.5, Codename "Stable Master"
 * Core stability improvements, lots of threading and memory usage related
   bugs have been resolved.
 * MySQL Database backend has been cleaned up.
 * Proper support for game clients of version 1.10.2 has been added.
 * Support for auras, pets, spells, talents, totems, etc. has been added
   or improved.
 * Cross-platform issues have been resolved, MaNGOS should now build on
   more platforms, including FreeBSD.

## Version 0.1, Codename "Lightbringer"
 * AI system
 * Grid system
 * Lots a game features working.
 * Lovely optimizations for the beloved server.

## Version 0.0.3, Codename "Mango Carpet"
 * Interim release, not gone public.

## Version 0.0.2, Codename "Library"
 * Most data conversions working.
 * Database cleanup.
 * More features working.
 * Many bugs fixed.

## Version 0.0.1, Codename "Endeavour"
 * Data converter added.
 * Build system improvements.
 * Bug fixing and code cleaning.
