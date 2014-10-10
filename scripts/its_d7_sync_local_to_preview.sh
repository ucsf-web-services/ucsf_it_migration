#!/bin/bash

# turn off migration modules
drush --yes @it.local7 dis ucsf_it_migration
drush --yes @it.local7 dis migrate_d2d_ui
drush --yes @it.local7 dis migrate_d2d
drush --yes @it.local7 dis migrate_ui
drush --yes @it.local7 dis migrate

# drop old preview db
drush --yes @it.preview sql-drop

# sync the local db up to preview
drush --yes --no-cache sql-sync @it.local7 @it.preview
# sync the local file system up to preview
drush --yes rsync @it.local7:%files @it.preview:%files

# uninstall migration modules in preview
drush --yes @it.preview pm-uninstall migrate_d2d_ui
drush --yes @it.preview pm-uninstall migrate_d2d
drush --yes @it.preview pm-uninstall migrate_ui
drush --yes @it.preview pm-uninstall migrate

# rebuild registry in preview
drush --yes @it.preview registry-rebuild

# correct site name
drush @it.preview vset site_name preview-it.ucsf.edu

# clear cache in preview
drush --yes @it.preview cc all

# re-enable migration modules in local
drush --yes @it.local7 en migrate
drush --yes @it.local7 en migrate_ui
drush --yes @it.local7 en migrate_d2d
drush --yes @it.local7 en migrate_d2d_ui
drush --yes @it.local7 en ucsf_it_migration
drush --yes @it.local7 cc all
