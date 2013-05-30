<?php
// $Id$

/**
 * @file
 * html for a single stop in a gtfs manager route
 *
 */
?>
<div class="<?php print $classes; ?>">
  <hgroup>
    <h2><?php print render($stop['stop_name']); ?></h2>
    <h4><?php print render($stop['stop_desc']); ?></h4>
  <hgroup>

  <p><b>Runs on: </b><?php print render($stop['days']); ?></p>
  <p><b>Active from: </b><?php print render($stop['start_date']); ?></p>
  <p><b>Inactive on: </b><?php print render($stop['end_date']); ?></p>
  <p><b>Arrives at: </b><?php print render($stop['arrival_time']); ?></p>
  <p><b>Departs at: </b><?php print render($stop['departure_time']); ?></p>
  <p><b>Sequence in route: </b><?php print render($stop['stop_sequence']); ?></p>
  <p><b>Lattitude: </b><?php print render($stop['stop_lat']); ?></p>
  <p><b>Longitude: </b><?php print render($stop['stop_long']); ?></p>
<?php print $edit_link; ?> | <?php print $delete_link; ?>
</div>