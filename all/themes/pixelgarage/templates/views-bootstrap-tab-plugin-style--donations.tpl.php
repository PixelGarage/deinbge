<div id="views-bootstrap-tab-<?php print $id ?>" class="<?php print $classes ?>">
  <ul class="nav nav-<?php print $tab_type?> <?php if ($justified) print 'nav-justified' ?>">
    <?php foreach ($tabs as $key => $tab): ?>
     <li>
       <a href="#tab-<?php print "{$id}-{$key}" ?>" class="<?php if ($key === 1) print 'active'?>" data-toggle="tab"><?php print $tab ?></a>
     </li>
    <?php endforeach ?>
  </ul>
  <div class="tab-content">
    <?php foreach ($rows as $key => $row): ?>
      <div id="tab-<?php print "{$id}-{$key}" ?>" class="tab-pane <?php if ($key === 1) print 'active'?>">
        <?php print $row ?>
      </div>
    <?php endforeach ?>
  </div>
</div>

