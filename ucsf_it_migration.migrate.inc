<?php

/**
 * Implements hook_migrate_api().
 */
function ucsf_it_migration_migrate_api() {

  // common arguments shared by all migrations
  $common_arguments = array(
    'source_version' => 6,
    'group_name' => 'ucsf it',
    'source_connection' => 'ucsf_it_legacy', // bogus value, see "source_database" for the real deal.
    'source_database' => array(
      'driver' => 'mysql',
      'database' => 'information_technology',
      'host' => 'localhost',
      'user' => 'legacy',
      'password' => variable_get('ucsf_it_migration_password', ''),
    ),
  );

  $api = array(
    'api' => 2,
    // define migration group
    'groups' => array(
      'ucsf it' => array(
        'title' => t("UCSF IT Website Migrations"),
      ),
    ),
    'migrations' => array(
      // register user role migration
      'D6Role' => $common_arguments + array(
        'class_name' => 'DrupalRole6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'group' => 'ucsf it',
        'machine_name' => 'Role',
      ),
      // register user migration
      'D6User' => $common_arguments + array(
        'class_name' => 'DrupalUser6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'machine_name' => 'User',
        'dependencies' => array('Role'),
      ),
    ),
  );

  // register vocabulary migrations
  $common_vocabulary_arguments = $common_arguments + array(
    'class_name' => 'DrupalTerm6Migration',
    'soft_dependencies' => array('User'),
  );

  $vocabulary_arguments = array(
    'D6ItsCategoriesVocabulary' => array(
      'description' => t('Migration of ITS categories from Drupal 6'),
      'machine_name' => 'ItsCategories',
      'source_vocabulary' => '2',  // D6 Vocabulary ID
      'destination_vocabulary' => 'its_categories',
    ),
    'D6NewsTermsVocabulary' => array(
      'description' => t('Migration of News terms from Drupal 6'),
      'machine_name' => 'NewsTerms',
      'source_vocabulary' => '6',  // D6 Vocabulary ID
      'destination_vocabulary' => 'news_terms',
    ),
  );

  foreach ($vocabulary_arguments as $key => $arguments) {
    $arguments += $common_vocabulary_arguments;
    $api['migrations'][$key] = $arguments;
  }

  return $api;
}
