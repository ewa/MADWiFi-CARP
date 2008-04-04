#!/bin/sh
sudo -v
sudo modprobe -r ath_pci ath_rate_sample ath_hal wlan_wep wlan_scan_sta wlan
pushd /
ssh andersoe@unferth.esl. "cd ltkern/inst; tar cv *"| sudo tar xv
sudo depmod -ae `uname -r`
popd
sudo modprobe ath_pci