#!/bin/bash

drush @it.local7 mreg
# remove email trigger before running user migration
drush @it.local7 sql-query "DELETE FROM information_technology2.trigger_assignments WHERE aid = 2674;"
# run user migration
drush @it.local7 mi D6User
# re-add email trigger after running user migration
drush @it.local7 sql-query "INSERT INTO information_technology2.trigger_assignments (hook, aid, weight) VALUES ('user_insert', '2674', 1);"
drush @it.local7 mi D6File
drush --yes @it.local7 dis pathauto
drush --yes @it.local7 dis linkchecker
drush --yes @it.local7 dis auto_nodetitle
drush @it.local7 mi D6StoryNode
drush @it.local7 mi D6StatusNode
drush @it.local7 mi D6PageNode
drush @it.local7 mi D6NewsNode
drush @it.local7 mi D6HowDoNode
drush @it.local7 mi D6ServiceNode
drush @it.local7 mi D6AdditionalInfoNode
drush @it.local7 mi D6TutorialNode
drush @it.local7 mi D6CategoryDescriptionNode
drush @it.local7 mi D6ProjectNode
drush @it.local7 mi D6ProjectStatusNode
drush @it.local7 mi D6PolicyNode
drush @it.local7 mi D6TeamNode
drush @it.local7 mi D6Redirect
drush --yes @it.local7 en auto_nodetitle
drush --yes @it.local7 en pathauto
drush --yes @it.local7 en smart_breadcrumb
drush --yes @it.local7 en smart_breadcrumb_menu
drush --yes @it.local7 en smart_breadcrumb_node
drush --yes @it.local7 en smart_breadcrumb_views
drush --yes @it.local7 en linkchecker

# populate authmap
drush @it.local7 sql-query "INSERT INTO information_technology2.authmap (uid, authname, module) ( SELECT DISTINCT mmu.destid1 AS uid, am.authname, am.module FROM information_technology2.migrate_map_d6user mmu JOIN information_technology.users u ON u.uid = mmu.sourceid1 JOIN information_technology.authmap am ON am.uid = u.uid);"

# fill node queue
drush @it.local7 sql-query "INSERT INTO information_technology2.nodequeue_nodes (qid, sqid, nid, position, timestamp) (SELECT 1, 1, n.nid, nn.position, nn.timestamp FROM information_technology.nodequeue_nodes nn JOIN information_technology2.migrate_map_d6projectnode mm ON mm.sourceid1 = nn.nid JOIN information_technology2.node n ON n.nid = mm.destid1);"

# add "About" link to main menu
drush @it.local7 sql-query "INSERT INTO menu_links (menu_name, plid, link_path, router_path, link_title, options, module, hidden, external, has_children, expanded, weight, depth, customized, p1, p2, p3, p4, p5, p6, p7, p8, p9, updated) (SELECT 'main-menu', '0', CONCAT('node/', n.nid), 'node/%', 'About', 'a:1:{s:10:\"attributes\";a:2:{s:5:\"title\";s:16:\"About IT at UCSF\";s:5:\"class\";a:1:{i:0;s:5:\"about\";}}}', 'menu', 0, 0, 0, 0, -47, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 FROM node n JOIN migrate_map_d6pagenode mm ON mm.destid1 = n.nid WHERE mm.sourceid1 = 32);";
drush @it.local7 sql-query " UPDATE menu_links SET p1 = mlid WHERE menu_name = 'main-menu' AND link_title = 'About';";


# add "All" link to current status menu
drush @it.local7 sql-query "INSERT INTO menu_links (menu_name, plid, link_path, router_path, link_title, options, module, hidden, external, has_children, expanded, weight, depth, customized, p1, p2, p3, p4, p5, p6, p7, p8, p9, updated) (SELECT 'menu-current-status', '0', CONCAT('node/', n.nid), 'node/%', 'All', 'a:1:{s:10:\"attributes\";a:0:{}}', 'menu', 0, 0, 0, 0, -47, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 FROM node n JOIN migrate_map_d6pagenode mm ON mm.destid1 = n.nid WHERE mm.sourceid1 = 501);";

drush @it.local7 sql-query " UPDATE menu_links SET p1 = mlid WHERE menu_name = 'menu-current-status' AND link_title = 'All';";

# add "Scheduled Maintenance Archive" link to Status Archives menu
drush @it.local7 sql-query "INSERT INTO menu_links (menu_name, plid, link_path, router_path, link_title, options, module, hidden, external, has_children, expanded, weight, depth, customized, p1, p2, p3, p4, p5, p6, p7, p8, p9, updated) (SELECT 'menu-status-archives', '0', CONCAT('node/', n.nid), 'node/%', 'Scheduled Maintenance', 'a:1:{s:10:\"attributes\";a:0:{}}', 'menu', 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 FROM node n JOIN migrate_map_d6pagenode mm ON mm.destid1 = n.nid WHERE mm.sourceid1 = 691);";

drush @it.local7 sql-query " UPDATE menu_links SET p1 = mlid WHERE menu_name = 'menu-status-archives' AND link_title = 'Scheduled Maintenance';";
