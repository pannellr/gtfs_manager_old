<?php
/*
 *
 */
?>

<h1>Active Transfers</h1>

<table class="active-transfers-table">
	<thead>
		<tr>
			<th>Stop Name</th>
			<th>From Route</th>
			<th>To Route</th>
			<th>Created By</th>
			<th>Created On</th>
			<th>Options</th>
		</tr>
	</thead>
	<tbody>

<?php foreach ($variables['transfers'] as $transfer) { ?>
<tr>
	<td><?php print $transfer->stop_name; ?></td>
	<td><?php print $transfer->from_route_name; ?></td>
	<td><?php print $transfer->to_route_name; ?></td>
	<td><?php print $transfer->created_by; ?></td>
	<td><?php print $transfer->created_at; ?></td>
	<td><?php print $transfer->ignore_link . '<br />' . $transfer->remove_link; ?></td>
</tr>
<?php } ?>

    </tbody>
</table>