<?php /* This file is part of Jeedom.
 *
 * Jeedom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Jeedom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jeedom. If not, see <http://www.gnu.org/licenses/>.
 */

if (!isConnect('admin')) {
	throw new Exception('401 Unauthorized');
}

?>
<script type="text/javascript" src="plugins/openzwave/3rdparty/vivagraph/vivagraph.min.js"></script>
<style media="screen" type="text/css">
    #graph_network {
        height: 80%;
        width: 90%;
        position: absolute;
    }
    #graph_network > svg {
        height: 100%;
        width: 100%
    }
    .noscrolling {
        width: 99%;
        overflow: hidden;
    }
    .table-striped {
        width: 90%;
    }
    .node-item {
        border: 1px solid;
    }
    .modal-dialog-center {
        margin: 0;
        position: absolute;
        top: 0%;
        left: 0%;
    }
    .node-primary-controller-color{
        color: #a65ba6;
    }
    .node-direct-link-color {
        color: #7BCC7B;
    }
    .node-remote-control-color {
        color: #00a2e8;
    }
    .node-more-of-one-up-color {
        color: #E5E500;
    }
    .node-more-of-two-up-color {
        color: #FFAA00;
    }
    .node-interview-not-completed-color {
        color: #979797;
    }
    .node-no-neighbourhood-color {
        color: #d20606;
    }
    .node-na-color {
        color: white;
    }
    .greeniconcolor {
        color: green;
    }
    .yellowiconcolor {
        color: #FFD700;
    }
    .rediconcolor {
        color: red;
    }
    #log {
        width: 100%;
        height: 700px;
        margin: 0px;
        padding: 0px;
        font-size: 16px;
        color: #fff;
        background-color: #300a24;
        overflow: scroll;
        overflow-x: hidden;
        font-size: 16px;
    }
    .console-out {
        padding-left: 20px;
        padding-top: 20px;
    }
    .bound-config {
        width: 100%;
        margin: 0px;
        padding: 0px;
    }
    .bound-config textarea {
        width: 100%;
        margin: 0px;
        padding: 20px;
        height: 700px;
        font-size: 14px;

    }

    .knockConf_table th
    {
	width: 10%;
    }

</style>
<div id='div_networkOpenzwaveAlert' style="display: none;"></div>
<div class='network' nid='' id="div_templateNetwork">
    <div class="container-fluid">
        <div id="content">
                <div id="config_knocks" class="tab-pane">
                    <table  class="table table-bordered table-condensed" style="width: 100%;position:relative;margin-top : 25px;">
                        <thead>
                        <tr class="knockConf_table_titles" style="background-color: #94ca02;">
                            <th colspan="1">{{Knock}}</th>
                            <th colspan="1">{{Nom du bracelet}}</th>
                            <th colspan="1">{{Nom du module}}</th>
                            <th colspan="1">{{Nombre de knock}}</th>
                        </tr>
                        </thead>
                        <tbody id="config_knock">


                <?php
			$bracelets = knocklet::getBraceletList();
			$modules =knocklet::getModuleList();
                        foreach(cmd::all() as $cmd)
                        {
                               echo  '<tr ID="'.$cmd->getId().'"><td><b>Equipement:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>'.eqLogic::byId($cmd->getEqLogic_Id())->getName()." <br><br><b>Commande:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>".$cmd->getName().'</td>';
 			       echo '<td><select class="sel_bracelet form-control" data-l1key="object_id"><option value="">{{Aucun}}</option>';
                               foreach ($bracelets as $object)
                               {
                                        if(knocklet::getTripletFromCmdId($cmd->getId())["braceletId"]==$object["id"])
                                        {
                                                echo '<option value="' . $object["id"] . '" selected>' . $object["name"] . '</option>';
                                        }
                                        else echo '<option value="' . $object["id"] . '">' . $object["name"] . '</option>';

                               }
			       if(knocklet::getTripletFromCmdId($cmd->getId())["braceletId"]=="*")
                                {
                               		echo '<option value="*" selected> Tous </option>';
				}else echo '<option value="*"> Tous </option>';

                  	       echo '</select></td>';

 			       echo '<td><select class="sel_module form-control" data-l1key="object_id"><option value="">{{Aucun}}</option>';

			       foreach ($modules as $object)
				{
					if(knocklet::getTripletFromCmdId($cmd->getId())["moduleId"]==$object["id"])
					{
						echo '<option value="' . $object["id"] . '" selected>' . $object["name"] . '</option>';	
					}
					else echo '<option value="' . $object["id"] . '">' . $object["name"] . '</option>';

				}

				if(knocklet::getTripletFromCmdId($cmd->getId())["moduleId"]=="*")
                                {
                                        echo '<option value="*" selected> Tous </option>';
                                }else echo '<option value="*"> Tous </option>';

                  	       echo '</select></td>';
			       

			       echo '<td><select class="sel_knock form-control"><option value="">{{Aucun}}</option>';

                               for ($x=2;$x<9;$x++)
                                {
					if(knocklet::getTripletFromCmdId($cmd->getId())["knocks"]==$x)
                                        {
                                                echo '<option value="'.$x.'" selected>'.$x.'</option>';
					}else   echo '<option value="'.$x.'">'. $x.' </option>';

                                }
                               echo '</select></td>';
                               echo  '</tr>';
                        }
                ?>
                        </tbody>
                    </table>
            	    <div class="btn btn-success eqLogicAction" id="saveCmds"><i class="fa fa-check-circle"></i>{{Sauvegarder}}</div>
                    <div id="graph-node-name"></div>
                </div>
            </div>
        </div>
    </div>
</div>
</div>
<?php include_file('desktop', 'knocklet', 'js', 'knocklet');?>
