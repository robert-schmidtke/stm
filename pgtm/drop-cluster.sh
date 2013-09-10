#!/bin/bash

sudo -u robert -H sh -c "psql -p 5433 -c 'DROP TABLE pgtm'"
sudo -u postgres -H sh -c "dropdb -p 5433 robert"
sudo -u postgres -H sh -c "dropuser -p 5433 robert"

sudo pg_ctlcluster 9.1 ram stop

sudo pg_dropcluster 9.1 ram
sudo umount /tmp/ram
sudo rm -rf /tmp/ram
