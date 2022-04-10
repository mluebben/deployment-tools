# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright 2022 Matthias Lübben <ml81@gmx.de>
#

all: deploy-shell deploy

clean:
	rm -f deploy-shell
	rm -f deploy

install: deploy-shell deploy
	install --owner=root --group=deploy --mode=750 deploy-shell /usr/local/bin
	install --owner=root --group=deploy --mode=4750 deploy /usr/local/bin

deploy-shell: deploy-shell.cpp
	g++ deploy-shell.cpp -o deploy-shell

deploy: deploy.cpp
	g++ deploy.cpp -o deploy
