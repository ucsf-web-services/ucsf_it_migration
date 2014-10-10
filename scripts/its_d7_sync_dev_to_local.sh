#!/bin/bash

# drop old local db
drush --yes @it.local7 sql-drop

# sync the dev db down to local
drush --yes --no-cache sql-sync @it.dev2 @it.local7
# sync the dev file system down to local

# fix files directory path
drush @it.local7 vset file_public_path "sites/it.ucsf.edu/files"

drush --yes rsync @it.dev2:%files @it.local7:%files
# adjust file perms on local
find ~/ucsfp/docroot/sites/it.ucsf.edu/files -type d -exec chmod 777 {} \;
find ~/ucsfp/docroot/sites/it.ucsf.edu/files -type f -exec chmod 666 {} \;

# turn on migration modules
drush --yes @it.local7 en migrate
drush --yes @it.local7 en migrate_ui
drush --yes @it.local7 en migrate_d2d
drush --yes @it.local7 en migrate_d2d_ui
drush --yes @it.local7 en ucsf_it_migration

# correct site name
drush @it.local7 vset site_name local7.it.ucsf.edu

# clear cache
drush --yes @it.local7 cc all

# register migrations
drush --yes @it.local7 mreg


