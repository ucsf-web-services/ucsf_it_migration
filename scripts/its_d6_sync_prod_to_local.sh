#!/bin/bash

# drop old local db
drush --yes @it.local sql-drop

# sync the production db down to local
drush --yes --no-cache sql-sync @it.prod @it.local

# fix files dir path
drush @it.local vset file_directory_path "sites/it.ucsf.edu/files"

# sync the production file system down to local
drush --yes rsync @it.prod:%files @it.local:%files

# disable aquia modules
#drush --yes @it.local dis acquia_agent acquia_search acquia_spi

# turn off caching, script aggregation etc
drush @it.local vset cache 0
drush @it.local vset block_cache 0
drush @it.local vset preprocess_css 0
drush @it.local vset preprocess_js 0

# correct site name
drush @it.local vset site_name local.it.ucsf.edu

# clear cache
drush --yes @it.local cc all

