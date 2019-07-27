# raspi-gameboy-battery

Clone repository and submodule

    git clone --recurse-submodules

Build raspidmx for pngview

    cd raspidmx && make && cd ..

Test manually (uses /dev/ttyACM0 as serial port)

    ./battery.py

Install systemd unit

    ln -s /root/raspi-gameboy-battery/battery.service /etc/systemd/system/battery.service
    systemctl daemon-reload
    systemctl enable battery
