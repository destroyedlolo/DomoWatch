<p align="center">
<img src="https://img.shields.io/github/last-commit/destroyedlolo/DomoWatch.svg?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/github/license/destroyedlolo/DomoWatch.svg?style=for-the-badge" />
</p>
<hr/>

# DomoWatch

The final goal of this project is to controle my smart home automation using TTGO TWatch.

In addition, the code has been made in **POO way** and being **very modular** (and commented as well) : should be a good starting point for new projects.

## Install

This project can be build using **Arduino IDE** and needs [Xinyuan-LilyGO /TTGO_TWatch_Library](https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library) and dependancies to be installed.

## Code convention

  - **header files** (\*.h) are containing class definition and **C++ ones** the implementation of heavy methods. They have the same name as the class implemented.
  - **Tl\* files** are for tiles definitions which are ordered inside Gui's constructor (**Gui.cpp**)
  - others are mostly C++ implementation of LvGL's objects (well subset of them)

## User guide

The **main tile** only displays the current date and time with a **status bar** containing *battery level* and a *step counter*. Clicking on it to reset.

  - On it's left, the *status tile* that is displaying some watch's figures (consumption, RAM, ...).
  - On it's bottom, the *setting tile* :
    - *brightness level* : take in account it's value is not stored but in memory. In case of a reboot, previous value is lost.

---

This code is inspirated from examples comming with TTGO_TWatch_Library made by Lewis He.
Some parts (icons) came from [sharandac/My-TTGO-Watch's project](https://github.com/sharandac/My-TTGO-Watch/)
