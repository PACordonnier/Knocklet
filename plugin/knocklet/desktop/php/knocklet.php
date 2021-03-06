<?php
if (!isConnect('admin')) {
	throw new Exception('{{401 - Accès non autorisé}}');
}
sendVarToJS('eqType', 'knocklet');
$eqLogics = eqLogic::byType('knocklet');
?>

<div class="row row-overflow">
    <div class="col-lg-2 col-md-3 col-sm-4">
        <div class="bs-sidebar">
            <ul id="ul_eqLogic" class="nav nav-list bs-sidenav">
               <!-- <a class="btn btn-default eqLogicAction" style="width : 100%;margin-top : 5px;margin-bottom: 5px;" data-action="add"><i class="fa fa-plus-circle"></i> {{Ajouter un template}}</a> -->
                <li class="filter" style="margin-bottom: 5px;"><input class="filter form-control input-sm" placeholder="{{Rechercher}}" style="width: 100%"/></li>
                <?php
foreach ($eqLogics as $eqLogic) {
	echo '<li class="cursor li_eqLogic" data-eqLogic_id="' . $eqLogic->getId() . '"><a>' . $eqLogic->getHumanName(true) . '</a></li>';
}
?>
           </ul>
      </div>
   </div>

   <div class="col-lg-10 col-md-9 col-sm-8 eqLogicThumbnailDisplay" style="border-left: solid 1px #EEE; padding-left: 25px;">

<legend><i class="fa fa-table"></i> {{Mes équipements}}</legend>

    <div class="eqLogicThumbnailContainer">
    <?php
foreach ($eqLogics as $eqLogic) {
        echo '<div class="eqLogicDisplayCard cursor" data-eqLogic_id="' . $eqLogic->getId().'"style="background-color:#ffffff;height:200px;margin-bottom : 10px;padding : 5px;border-radius: 2px;width : 160px;margin-left : 10px;" >';

        echo "<center>";
	echo '<img src="' . $eqLogic->getImgFilePath() . '" height="105" width="95" />';
        echo "</center>";

        echo '<span style="font-size : 1.1em;position:relative; top : 15px;word-break: break-all;white-space: pre-wrap;word-wrap: break-word;"><center>' . $eqLogic->getHumanName(true, true) . '</center></span>';
        echo '</div>';
}
?>


</div>

    <legend><i class="fa fa-cog"></i> {{Gestion}}</legend>
    <div style="text-align: center;width: 10%;float: left;">
	<div>
            <i class="fa fa-sitemap" id="bt_recupCmd" style="font-size : 6em;color:#767676;"></i>
        </div>
        <span style="font-size : 1.1em;position:relative; top : 23px;word-break: break-all;word-wrap: break-word;color:#767676">
                <div>{{Configuration}}</div>

                <p style="margin:0; padding:0;">
                        <div>{{des commandes}}</div>
                </p>
        </span>
    
    </div>

    <div style="text-align: center;width: 10%;float: left;">
        <div>
            <i class="fa fa-sitemap" id="bt_recupScio" style="font-size : 6em;color:#767676;"></i>
        </div>
        <span style="font-size : 1.1em;position:relative; top : 23px;word-break: break-all;word-wrap: break-word;color:#767676">
                <div>{{Configuration}}</div>

                <p style="margin:0; padding:0;">
                        <div>{{des scénarios}}</div>
                </p>
        </span>

    </div>


</div>

<div class="col-lg-10 col-md-9 col-sm-8 eqLogic" style="border-left: solid 1px #EEE; padding-left: 25px;display: none;">
    <form class="form-horizontal">
        <fieldset>
            <legend><i class="fa fa-arrow-circle-left eqLogicAction cursor" data-action="returnToThumbnailDisplay"></i> {{Général}}  <i class='fa fa-cogs eqLogicAction pull-right cursor expertModeVisible' data-action='configure'></i></legend>
            <div class="form-group">
                <label class="col-sm-3 control-label">{{Nom de l'équipement}}</label>
                <div class="col-sm-3">
                    <input type="text" class="eqLogicAttr form-control" data-l1key="id" style="display : none;" />
                    <input type="text" class="eqLogicAttr form-control" data-l1key="name" placeholder="{{Nom de l'équipement}}"/>
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-3 control-label" >{{Objet parent}}</label>
                <div class="col-sm-3">
                    <select id="sel_object" class="eqLogicAttr form-control" data-l1key="object_id">
                        <option value="">{{Aucun}}</option>
                        <?php
foreach (object::all() as $object) {
	echo '<option value="' . $object->getId() . '">' . $object->getName() . '</option>';
}
?>
                   </select>
               </div>
           </div>
           <div class="form-group">
            <label class="col-sm-3 control-label" >{{Activer}}</label>
            <div class="col-sm-9">
               <input type="checkbox" class="eqLogicAttr bootstrapSwitch" data-label-text="{{Activer}}" data-l1key="isEnable" checked/>
               <input type="checkbox" class="eqLogicAttr bootstrapSwitch" data-label-text="{{Visible}}" data-l1key="isVisible" checked/>
           </div>
       </div>
	<?php   echo '<div class"form-group">';
		echo '<label class="col-sm-3 control-label">{{Adresse}}</label>';
                echo '<div class="col-sm-3">';
		echo '<input type="text" class="eqLogicAttr form-control" data-l1key="configuration" data-l2key="MAC" placeholder="Adresse" readonly/>';
		echo '</div>'; 
		echo '</div>';
 ?>

</fieldset>
</form>


<form class="form-horizontal">
    <fieldset>
        <div class="form-actions" style="margin-left: 25%;"> 
            <a class="btn btn-success eqLogicAction" data-action="save">{{Sauvegarder}}</a>
            <a class="btn btn-success eqLogicAction" data-action="remove"> {{Supprimer}}</a>
        </div>
    </fieldset>
</form>
</div>

</div>

<?php include_file('desktop', 'knocklet', 'js', 'knocklet');?>
<?php include_file('core', 'knocklet', 'class', 'knocklet');?>
<?php include_file('core', 'plugin.template', 'js');?>
