#!/bin/bash

mkdir -p /tmp/ram/psql
sudo mount -t tmpfs -o size=1024M tmpfs /tmp/ram/
sudo pg_createcluster -d /tmp/ram/psql 9.1 ram

sudo pg_ctlcluster 9.1 ram start -o '-F'

sudo -u postgres -H sh -c "createuser -p 5433 -s robert"
sudo -u postgres -H sh -c "createdb -p 5433 robert -O robert"
sudo -u robert -H sh -c "psql -p 5433 -c 'CREATE TABLE pgtm (addr char(18) PRIMARY KEY, valu bigint)'"
