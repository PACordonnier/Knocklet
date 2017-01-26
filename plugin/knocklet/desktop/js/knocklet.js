
/* This file is part of Jeedom.
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


$("#table_cmd").sortable({axis: "y", cursor: "move", items: ".cmd", placeholder: "ui-state-highlight", tolerance: "intersect", forcePlaceholderSize: true});
/*
 * Fonction pour l'ajout de commande, appellé automatiquement par plugin.template
 */
function addCmdToTable(_cmd) {
    if (!isset(_cmd)) {
        var _cmd = {configuration: {}};
    }
    if (!isset(_cmd.configuration)) {
        _cmd.configuration = {};
    }
    alert("I am an alert box!");
    var tr = '<tr class="cmd" data-cmd_id="' + init(_cmd.id) + '">';
    tr += '<td>';
    tr += '<span class="cmdAttr" data-l1key="id" style="display:none;"></span>';
    tr += '<input class="cmdAttr form-control input-sm" data-l1key="name" style="width : 140px;" placeholder="{{Nom?}}">';
    tr += '</td>';
    tr += '<td>';
    tr += '<span class="type" type="' + init(_cmd.type) + '">' + jeedom.cmd.availableType() + '</span>';
    tr += '<span class="subType" subType="' + init(_cmd.subType) + '"></span>';
    tr += '</td>';
    tr += '<td>';
    if (is_numeric(_cmd.id)) {
        tr += '<a class="btn btn-default btn-xs cmdAction expertModeVisible" data-action="configure"><i class="fa fa-cogs"></i></a> ';
        tr += '<a class="btn btn-default btn-xs cmdAction" data-action="test"><i class="fa fa-rss"></i> {{Tester}}</a>';
    }
    tr += '<i class="fa fa-minus-circle pull-right cmdAction cursor" data-action="remove"></i>';
    tr += '</td>';
    tr += '</tr>';
    $('#table_cmd tbody').append(tr);
    $('#table_cmd tbody tr:last').setValues(_cmd, '.cmdAttr');
    if (isset(_cmd.type)) {
        $('#table_cmd tbody tr:last .cmdAttr[data-l1key=type]').value(init(_cmd.type));
    }
    jeedom.cmd.changeType($('#table_cmd tbody tr:last'), init(_cmd.subType));
}


$('#bt_recupCmd').on('click', function () {
	 $('#md_modal').dialog({title: "{{Configuration des knocks}}"});
    $('#md_modal').load('index.php?v=d&plugin=knocklet&modal=configKnock').dialog('open');


});



$('#bt_recupScio').on('click', function () {
         $('#md_modal').dialog({title: "{{Configuration des scénarios}}"});
    $('#md_modal').load('index.php?v=d&plugin=knocklet&modal=configScenarios').dialog('open');

});




$('#bt_ajout_bracelet').on('click', function () {
        
	$('#md_modal').dialog({title: "{{Ajouter un bracelet}}"});
    	$('#md_modal').load('index.php?v=d&plugin=knocklet&modal=scanBracelet').dialog('open');
	

	
	$.ajax({
     		url: 'plugins/knocklet/core/ajax/knocklet.ajax.php',
     		type: 'POST',
     		data: "startScan",
     		success: function (data) {
			
			var data_table=[];
			var data_split=data.split("\n")
			var row;
                        var cell, btn;

			console.log(data);
			console.log(data_split);

                        var table_brac= document.getElementById("tbody_scanBracelet");
                        var table_mod= document.getElementById("tbody_scanModule");

			
			for (var i = 0; i< data_split.length; i++) {
 				if(data_split[i]!==""){
					if(json_decode(data_split[i])["type"]=="bracelet"){

						row = table_brac.insertRow(0);
						cell  = row.insertCell(0);
						cell.innerHTML = json_decode(data_split[i])["id"];		

					}
					else if(json_decode(data_split[i])["type"]=="module"){

						row = table_mod.insertRow(0);
                                                
						cell = row.insertCell(0);
						btn = row.insertCell(1);
							
						cell.innerHTML = json_decode(data_split[i])["id"];

					}
					else console.log("type non reconnu");


					data_table.push(json_decode(data_split[i]));
					
					console.log(json_decode(data_split[i]));
				}
			}
			console.log(data_table);







			//cell1.innerHTML = data;
   	 		//cell2.innerHTML = data;

			//var row2 = table_mod.insertRow(0);
                        //cell = row.insertCell(0);
                        //cell = row.insertCell(1);

                        //cell1.innerHTML = data;
                        //cell2.innerHTML = data;

            	},
     		error: function(){alert("Erreur dans la demande de scan");}
	});

});




$('#saveCmds').on('click',function (){

$('body').append('<div id="jqueryLoadingDiv"><div class="overlay"></div><i class="fa fa-cog fa-spin loadingImg"></i></div>');

var returnTable= [];
var container = document.getElementById('config_knock');
var bracelets = document.getElementsByClassName('sel_bracelet');
var modules = document.getElementsByClassName('sel_module');
var knocks = document.getElementsByClassName('sel_knock');
var cmds = container.getElementsByTagName('tr');
var j=0;
var table_cmds = [];


for (j = 0; j < cmds.length; j++) {

        if(bracelets[j].options[bracelets[j].selectedIndex].value!=="" && modules[j].options[modules[j].selectedIndex].value !== "" && knocks[j].options[knocks[j].selectedIndex].text !== "Aucun"){
                returnTable[cmds[j].id] = [bracelets[j].options[bracelets[j].selectedIndex].value, modules[j].options[modules[j].selectedIndex].value, knocks[j].options[knocks[j].selectedIndex].text];
        }
	else returnTable[cmds[j].id] = null;	
	
    }

$.ajax({
	type: "POST",
	url: "plugins/knocklet/core/ajax/knocklet.ajax.php",
	data: {
		action: "saveCmd",
		json: JSON.stringify(returnTable),
	},
	dataType: 'json',
        global: false,
        error: function (request, status, error) {
            handleAjaxError(request, status, error);
        },
        success: function (data) {
		//DONOTTOUCH, si on appelle la fonction que une seule fois, ça marche pas 
		$('#jqueryLoadingDiv').remove();
		$('#jqueryLoadingDiv').remove();
		
	}
});

});




$('#saveScios').on('click',function (){

$('body').append('<div id="jqueryLoadingDiv"><div class="overlay"></div><i class="fa fa-cog fa-spin loadingImg"></i></div>');

var returnTable= [];
var container = document.getElementById('config_scenarios');
var bracelets = document.getElementsByClassName('sel_bracelet');
var modules = document.getElementsByClassName('sel_module');
var knocks = document.getElementsByClassName('sel_knock');
var cmds = container.getElementsByTagName('tr');
var j=0;
var table_cmds = [];
console.log(cmds);

for (j = 0; j < cmds.length-1; j++) {
	


	console.log(cmds[j]);
        if(bracelets[j].options[bracelets[j].selectedIndex].value!=="" && modules[j].options[modules[j].selectedIndex].value !== "" && knocks[j].options[knocks[j].selectedIndex].text !== "Aucun"){
                returnTable[cmds[j+1].id] = [bracelets[j].options[bracelets[j].selectedIndex].value, modules[j].options[modules[j].selectedIndex].value, knocks[j].options[knocks[j].selectedIndex].text];
        }
        else returnTable[cmds[j+1].id] = null;

    }
console.log(returnTable);

$.ajax({
        type: "POST",
        url: "plugins/knocklet/core/ajax/knocklet.ajax.php",
        data: {
                action: "saveScio",
                json: JSON.stringify(returnTable),
        },
        dataType: 'json',
        global: false,
        error: function (request, status, error) {
            handleAjaxError(request, status, error);
        },
        success: function (data) {
                //DONOTTOUCH, si on appelle la fonction que une seule fois, ça marche pas
                $('#jqueryLoadingDiv').remove();
                $('#jqueryLoadingDiv').remove();

        }
});

});
