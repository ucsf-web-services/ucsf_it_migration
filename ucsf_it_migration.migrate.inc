<?php

/**
 * Implements hook_migrate_api().
 */
function ucsf_it_migration_migrate_api() {

  // common arguments shared by all migrations
  $common_arguments = array(
    'source_version' => 6,
    'group_name' => 'ucsf_it',
    'source_connection' => 'legacy',
  );

  $api = array(
    'api' => 2,
    // define migration group
    'groups' => array(
      'ucsf_it' => array(
        'title' => t("UCSF IT Website Migrations"),
      ),
    ),
    'migrations' => array(
      // register user role migration
      'D6Role' => $common_arguments + array(
        'class_name' => 'DrupalRole6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'role_mappings' => array(
          'admin' => 'administrator',
        ),
      ),
      // register user migration
      'D6User' => $common_arguments + array(
        'class_name' => 'DrupalUser6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'dependencies' => array('D6Role'),
        'role_migration' => 'D6Role',
      ),
    ),
  );

  // register vocabulary migrations
  $common_vocabulary_arguments = $common_arguments + array(
    'class_name' => 'UcsfItTerm6Migration',
    'soft_dependencies' => array('D6User'),
  );

  $vocabulary_arguments = array(
    'D6ItsCategoriesVocabulary' => array(
      'description' => t('Migration of ITS categories from Drupal 6'),
      'source_vocabulary' => '2',  // D6 vocabulary id
      'destination_vocabulary' => 'its_categories', // D7 vocabulary machine name
    ),
    'D6NewsTermsVocabulary' => array(
      'description' => t('Migration of News terms from Drupal 6'),
      'source_vocabulary' => '6',
      'destination_vocabulary' => 'news_terms',
    ),
  );

  foreach ($vocabulary_arguments as $key => $arguments) {
    $arguments += $common_vocabulary_arguments;
    $api['migrations'][$key] = $arguments;
  }

  // register node migrations
  $node_arguments = $common_arguments + array(
      'user_migration' => 'D6User',
      'default_uid' => 0,
  );

  $api['migrations']['D6StoryNode'] = $node_arguments + array(
    'source_type' => 'story',
    'destination_type' => 'article',
    'description' => t('Migration of Story nodes from Drupal 6'),
    'class_name' => 'UcsfItStoryNode6Migration',
    'dependencies' => array('D6ItsCategoriesVocabulary'),
  );

  return $api;
}
