<?php
/**
 * Holds the nodes import functions.
 */

/* =============================================================================
 *   Constants
 * ========================================================================== */
/**
 * A bit flag used to let us know if a node type is in the database.
 */
define('PXLRAFFLE_NODE_IN_DATABASE', 0x01);

/**
 * A bit flag used to let us know if a node type is in code and not imported yet.
 */
define('PXLRAFFLE_NODE_IN_CODE', 0x02);


/**
 * Get all nodes located in the nodes structure directory.
 */
function pxlraffle_nodes_default() {
  // get all nodes in code (hold in files ending with .node)
  $nodes = array();
  $files = file_scan_directory(drupal_get_path('module', 'pxlraffle') . '/structures/nodes', '#\.node$#');
  foreach ($files as $filepath => $fobj) {
    if (($file = file_get_contents($filepath)) !== false) {
      // delete php tag, if any
      $file = str_replace('<?php', '', $file);

      // import node data
      require $filepath;

      // Add node to default nodes listed in the views overview page.
      if (isset($data) && is_array($data)) {
        // get export timestamp in file
        $code_exported = isset($export_time) ? $export_time : 0;

        // Create array entries.
        foreach ($data['bundles'] as $key => $bundle) {
          $title = $key; // fallback for title is machine name
          if (is_object($bundle)) {
            $title = $bundle->name;
          }
          elseif (is_array($bundle)) {
            $title = $bundle['name'];
          }

          // add node bundle data
          $nodes[$key] = array(
            'title' => $title,
            'storage' => PXLRAFFLE_NODE_IN_CODE,
            'code_exported' => $code_exported,
            'data' => $data,
            'values' => array('macro' => $file), // compatibility to bundle_copy
          );
        }
      }
    }
  }
  return $nodes;
}
