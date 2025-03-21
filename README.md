# Linux Kernel Module Dice
This project is a Linux Kernel Module which can be used to create random integers between 1 and a dice limit.

# Development
A Vagrant file has been provided to assist with setting up a simple vagrant VM for use on non-linux machines.
This Vagrant box is setup to use Ubuntu Bionic (18.04.5) 64Bit, as well as use Ansible to provision the machine with the required dependencies.

If you are using a clean linux machine the following commands can be run to install the required dependencies

```bash
sudo apt-get install libelf-dev build-essential linux-headers-$(uname -r)
```

You may need to adapt your install command if your distribution does not include apt, or package names vary

# Installation
To install the module, perform the following commands. Root may be required depending on your setup. modprobe may also be required in place of insmod and rmmod depending on your system.

## Install
Once compiled, the Kernel Module can be installed using the following command

```bash
insmod lkm-dice.ko
```

To add the character device manually, the following command can be used

```bash
mknod /dev/dice c <major> 0
```

where `<major>` is the major number which can be found by running

```bash
dmesg
```

## Uninstall
To remove the device in the /dev folder, you can remove the file using the rm command, e.g.

```bash
rm /dev/dice
```

To uninstall the module, the following command can be executed

```bash
rmmod lkm-dice.ko
```