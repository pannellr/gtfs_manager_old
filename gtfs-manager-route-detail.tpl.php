<?php
/**
 * @file route detail page template
 */
?>
<div class="<?php print $classes; ?>">
   <p><b>Short name: </b><?php print $route->route_short_name; ?></p>
   <p><b>Long name: </b><?php print $route->route_long_name; ?></p>
   <p><b>Description: </b><?php print $route->route_desc; ?></p>
   <p><b>Website URL: </b><?php print $route->route_url; ?></p>
   <p><b>Transportation type: </b><?php print $route->transportation_type_name; ?></p>
   <p><em>Created by <?php print $route->created_by; ?> on <?php print $creation_date; ?></em></p>
   <?php print $links; ?>
</div>