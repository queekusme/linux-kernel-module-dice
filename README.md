# Linux Kernel Module Dice
This project is a Linux Kernel Module which can be used to create random integers between 1 and a dice limit.

# Development
A Vagrant file has been provided to assist with setting up a simple vagrant VM for use on non-linux machines.
This Vagrant box is setup to use Ubuntu Bionic (18.04.5) 64Bit, as well as use Ansible to provision the machine with the required dependencies.

If you are using a clean linux machine the following commands can be run to install the required dependencies

```bash
sudo apt-get install libelf-dev build-essential inux-headers-$(uname -r)
```

You may need to adapt your install command if your distribution does not include apt, or package names vary