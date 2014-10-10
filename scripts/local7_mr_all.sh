#!/bin/bash
drush @it.local7 mreg
drush @it.local7 mr D6Redirect
drush @it.local7 mr D6TeamNode
drush @it.local7 mr D6PolicyNode
drush @it.local7 mr D6ProjectStatusNode
drush @it.local7 mr D6ProjectNode
drush @it.local7 mr D6CategoryDescriptionNode
drush @it.local7 mr D6TutorialNode
drush @it.local7 mr D6AdditionalInfoNode
drush @it.local7 mr D6ServiceNode
drush @it.local7 mr D6HowDoNode
drush @it.local7 mr D6NewsNode
drush @it.local7 mr D6PageNode
drush @it.local7 mr D6StatusNode
drush @it.local7 mr D6StoryNode
drush @it.local7 mr D6File
drush @it.local7 mr D6User

# clear outmap table
drush @it.local7 sql-query "DELETE FROM authmap"

# empty node queue
drush @it.local7 sql-query "DELETE FROM nodequeue_nodes WHERE qid  = 1;"

# remove "About" link from main menu
drush @it.local7 sql-query "DELETE FROM menu_links WHERE menu_name = 'main-menu' AND link_title = 'About';"

# remove "All" link from Current Status menu
drush @it.local7 sql-query "DELETE FROM menu_links WHERE menu_name = 'menu-current-status' AND link_title = 'All';"

# remove "Scheduled Maintenance Archive" link from Status Archives menu
drush @it.local7 sql-query "DELETE FROM menu_links WHERE menu_name = 'menu-status-archives' AND link_title = 'Scheduled Maintenance';"
