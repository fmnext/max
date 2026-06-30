# ForzaTech Importer for Autodesk 3ds Max
Translator plugin for importing ForzaTech 3D assets (.zip, .modelbin) into Autodesk 3ds Max.

> [!IMPORTANT]
> Full game installation, decrypted database and untouched assets are required to take full advantage of the features of this plugin.

## Features
* Directly .zip reading support
* Textures extraction/conversion
* Material extraction/conversion
* Manufacturer colors extraction/conversion
* Full animation support when available
* Upgrade management/selection
* Fast data translation
* Quick sort filter 
* Quad and tri mesh support

### Supported products
* Autodesk 3ds Max 2023
* Autodesk 3ds Max 2024
* Autodesk 3ds Max 2025
* Autodesk 3ds Max 2026
* Autodesk 3ds Max 2027

### Supported games
* Forza Motorsport 5
* Forza Motorsport 6 / Apex
* Forza Motorsport 7
* Forza Motorsport 2023
* Forza Horizon 2
* Forza Horizon 3
* Forza Horizon 4
* Forza Horizon 5
* Forza Horizon 6

## Project setup and build

This code is designed to build with Visual Studio 2022, Visual Studio 2026 or later. Use of the Windows 11 April 2025 Update SDK (22621) or later is required for Visual Studio.

Necessary workloads
* Desktop development with C++
* Game development with C++

## Requirements
1. [Git](https://git-scm.com/)
2. [Visual Studio 2022](https://aka.ms/vs/17/release/vs_enterprise.exe)
3. [Autodesk 3ds Max SDK](https://aps.autodesk.com/developer/overview/3ds-max-api)
4. [Qt](https://www.qt.io)
5. [Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022)
6. [Autodesk 3ds Max](https://www.autodesk.com/products/3ds-max/overview)

## Clone
```
git clone https://github.com/fmnext/max.git --recursive
```

### Qt Version by product year
>Qt License is required to download and setup the environment.

| Autodesk Product Year  | Qt Version |
|-------|-----|
| 2027 | 6.8.3 |
| 2025 - 2026 | 6.5.3 |
| 2024 | 5.15.2 |
| 2023 | 5.15.1 |

## Build
* Install requirements.
* Open main solution `max.sln`.
* Extensions > Qt VS Tools > Qt Versions > Click Autodetect, press OK and re-open solution.
* On the toolbar, choose Release.
* Single version: right sidebar, RMB the desired version > Build.
* All versions: right sidebar, RMB the solution > Build Solution. (all SDK and Qt must be installed)

## Credits
* All research archived on this project is the solely result of [Doliman100's](https://github.com/Doliman100) work.


*********


### DISCLAIMER

The inclusion of Forza's logo, products, or services in any materials or content does not imply endorsement or affiliation with Microsoft Corporation, unless specifically stated.

### Notices
All content and source code for this project are subject to the terms of the [GPL License](https://).

### Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft trademarks or logos is subject to and must follow [Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks/usage/general). Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship. Any use of third-party trademarks or logos are subject to those third-party's policies.

© 2026, Microsoft Corporation. Forza and Forza Logo are trademarks or registered trademarks of Microsoft Corporation.

© 2026, Autodesk, Inc. Maya and 3ds Max are trademarks or registered trademarks of Autodesk, Inc. in the USA and elsewhere.

© 2026, The Qt Company. Qt Logo is a trademark or registered trademark of Qt Group.