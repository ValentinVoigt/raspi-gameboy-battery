# raspi-gameboy-battery

This repository is used on a self-made handheld Raspberry Pi gameboy clone. It uses *pngview* from a [modified raspidmx](https://github.com/ValentinVoigt/raspidmx) repository to display a battery icon and battery voltage on screen, even while playing.

Install prerequisites

    apt-get install python3-serial

Clone repository and submodule

    git clone --recurse-submodules https://github.com/ValentinVoigt/raspi-gameboy-battery

Build raspidmx for pngview

    cd raspidmx && make && cp lib/libraspidmx.so.1 /usr/lib/ && cd ..

Test manually (uses /dev/ttyACM0 as serial port)

    ./battery.py

Install systemd unit

    ln -s /root/raspi-gameboy-battery/battery.service /etc/systemd/system/battery.service
    systemctl daemon-reload
    systemctl enable battery
