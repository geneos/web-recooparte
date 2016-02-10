<?php
/**
 * @file
 * Media Gallery Extras utilities, in it's own file to be more lightweight.
 */

/**
 * Data entry and confirm form for moving an item.
 *
 * FAPI form.
 *
 * @param array $form
 *   FAPI
 * @param array $form_state
 *   FAPI
 * @param object $node
 *   Gallery
 * @param object $file
 *   Media item
 * @param string|int $position
 *   Position in the list to move it to.
 *
 * @return array
 *   FAPI form.
 */
function media_gallery_move_to_position_form($form, &$form_state, $node, $file, $position = 'top') {
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $file->fid,
  );
  $items = &$node->media_gallery_file[LANGUAGE_NONE];
  $form['position'] = array(
    '#title' => t('Position'),
    '#type' => 'textfield',
    '#default_value' => $position,
    '#size' => 3,
    '#description' => t('Enter a number from 1..%n, "top" or "bottom"', array('%n' => count($items))),
    '#element_validate' => array('media_gallery_position_validate'),
  );
  return confirm_form($form,
    t('Are you sure you want to reposition %file within the gallery %gallery?', array('%file' => $file->filename, '%gallery' => $node->title)),
    "media-gallery/detail/{$node->nid}/{$file->fid}",
    '',
    t('Move file')
  );
}

/**
 * Implements hook_validate().
 *
 * @see media_gallery_move_to_position_form()
 */
function media_gallery_position_validate($element, &$form_state) {
  $value = $element['#value'];
  if (!is_numeric($value) && $value != 'top' && $value != 'bottom') {
    form_error($element, t('Invalid %name. Please enter an integer, "top" or "bottom"', array('%name' => $element['#title'])));
  }
}

/**
 * Submit handler for removing a media item from a gallery.
 *
 * @see media_gallery_move_to_position_form()
 */
function media_gallery_move_to_position_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);
  $file = file_load($form_state['values']['fid']);
  $position = $form_state['values']['position'];

  media_gallery_extras_move_item_to_position($node, $file, $position);
  drupal_set_message(t('The file %file was moved to %position within the gallery.', array('%file' => $file->filename, '%position' => $position)));
  $form_state['redirect'] = "node/{$node->nid}";
}

/**
 * Config form for global media gallery settings.
 *
 * media_gallery provides NO global settings, so I have to do my own.
 *
 * A FAPI settings_form def.
 */
function media_gallery_extras_settings_form() {
  $form = array();

  $form['media_gallery_extras_add_items_order'] = array(
    '#title' => t("When adding media to a gallery, append new content to:"),
    '#default_value' => variable_get('media_gallery_extras_add_items_order', 'bottom'),
    '#type' => 'radios',
    '#options' => array(
      'top' => 'Top',
      'bottom' => 'Bottom',
    ),
  );

  return system_settings_form($form);
}
