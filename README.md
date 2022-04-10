# Deployment tools for applications

## Installation

Prepare debian for compiling and linking c++ prorams.

    $ sudo apt install build-essential

Compiling the deployment tools

    $ make

Register deploy-shell as a valid shell by appending it to /etc/shells:

    $ sudo echo "/usr/local/bin/deploy-shell" >>/etc/shells

Create the deployment group "deploy" and user "deploy"

    $ sudo addgroup deploy
    $ sudo adduser --shell /usr/local/bin/deploy-shell --ingroup deploy --disabled-password deploy

Install the deployment tools into /usr/local/bin

    $ sudo make install

Now, verify that we have our deploy-shell and deploy commands

    $ ll /usr/local/bin/{deploy-shell,deploy}

The output should yield something like this

    -rwsr-x--- 1 root deploy  9080 Apr 10 12:22 /usr/local/bin/deploy
    -rwxr-x--- 1 root deploy 13280 Apr 10 12:22 /usr/local/bin/deploy-shell

The user root should own the files with full access, the deploy program has the setuid bit set. The files belong to the group deploy with read and execute permissions so anyone in the group deploy can invoke them but not change them. All others have no business with these files and should not have any priviliges.

Create a ssh key. I did it with PuTTYgen. What you need is the private key and the public key to be used for the authorized file. We assume you have the files deploy.key with the private key and deploy.pub with the public key. (Hint: This paragraph needs a bit more explaining)


Enable login for the deploy user via ssh

    $ sudo mkdir /home/deploy/.ssh
    $ sudo cat deploy.pub >>/home/deploy/.ssh/authorized_keys
    $ sudo chown deploy.deploy -R /home/deploy/.ssh
    $ sudo chmod 700 /home/deploy/.ssh
    $ sudo chmod 600 /home/deploy/.ssh/authorized_keys

Now it's time to setup the /etc/deploy directory

    $ sudo mkdir /etc/deploy
    $ sudo cp /etc/deploy/templates/deploy.sh.template /etc/deploy/deploy.sh
    $ sudo chown root.root /etc/deploy/deploy.sh
    $ sudo chmod 700 /etc/deploy/deploy.sh

Feel free to adapt deploy.sh to your needs. It's just a bash shell script that receives a zip file from stdin or the from a regulare file. Ensure we have a regulare zip file and delegates to an installer script. The installer script receives the file name as first argument.

Create your first static site

    $ sudo mkdir /srv/www/my-static-site
    $ sudo mkdir /srv/www/my-static-site/htdocs

Copy over one of the installer templates and make it only executable by root

    $ sudo cp /etc/deploy/templates/installer/static-site-installer.sh /srv/www/my-static-site/install.sh
    $ sudo chown root.root /srv/www/my-static-site/install.sh
    $ sudo chmod 700 /srv/www/my-static-site/install.sh

Register the installer for the deployment tool

    $ sudo ln --symbolic /srv/www/my-static-site/install.sh /etc/deploy/installer/my-static-site
   
Deploy the contents of a zip file with the deployment tool

    $ sudo deploy -c my-static-site -i some-zip-file-with-static-site-contents.zip

Deploy via ssh to my-static-site

    $ cat some-zip-file-with-static-site-contents.zip | ssh -i your-openssh-private-key.key deploy@deployment-host-name.de deploy my-static-site

By adapting and changing the installation script every scenario is possible, e.g. starting and stopping service, keeping backups, copy local configuration files, etc. Even building the entire application or parts of it. Send over a deb package and use apt and so forth.

