<?php
/**
 * @page show the details for one agency
 * needs rework
 */
?>



<div class="<?php print $classes; ?>">
   <p><b>Website URL: </b><?php print $agency->agency_url; ?></p>
   <p><b>Phone Number: </b><?php print $agency->agency_phone; ?></p>
   <p><b>Email: </b><?php print $agency->agency_email; ?></p>
   <p><b>Information: </b><?php print $agency->agency_info; ?></p>
   <p><em>Created by <?php print $agency->created_by; ?> on <?php print $creation_date; ?></em></p>
 <?php print $links; ?>
</div>