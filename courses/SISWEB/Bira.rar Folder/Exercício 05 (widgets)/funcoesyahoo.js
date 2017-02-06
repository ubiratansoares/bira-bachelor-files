YAHOO.namespace("example.container");

//evento do botão cancelar do container
YAHOO.util.Event.onDOMReady(function () {
	
	var handleCancel = function() {
		this.cancel();
	};

    // Remove progressively enhanced content class, just before creating the module
    YAHOO.util.Dom.removeClass("dialog1", "yui-pe-content");
	YAHOO.util.Dom.removeClass("dialog2", "yui-pe-content");

	// Instanciação do Dialog1  e Dialog2
	YAHOO.example.container.dialog1 = new YAHOO.widget.Dialog("dialog1", 
							{ width : "30em",
							  fixedcenter : true,
							  visible : false, 
							  constraintoviewport : true,
							  buttons : [ { text:"Fechar", handler:handleCancel } ]
							});
	
	YAHOO.example.container.dialog2 = new YAHOO.widget.Dialog("dialog2", 
							{ width : "30em",
							  fixedcenter : true,
							  visible : true, 
							  constraintoviewport : true,
							  buttons : [ { text:"Fechar", handler:handleCancel } ]
							});
							
	// Renderizaçao do Dialog1 e Dialog2
	YAHOO.example.container.dialog1.render();
	YAHOO.example.container.dialog2.render();
	
	YAHOO.util.Event.addListener("show", "click", YAHOO.example.container.dialog1.show, YAHOO.example.container.dialog1, true);
	YAHOO.util.Event.addListener("hide", "click", YAHOO.example.container.dialog1.hide, YAHOO.example.container.dialog1, true);
	
	YAHOO.util.Event.addListener("show1", "click", YAHOO.example.container.dialog2.show, YAHOO.example.container.dialog2, true);
	YAHOO.util.Event.addListener("hide1", "click", YAHOO.example.container.dialog2.hide, YAHOO.example.container.dialog2, true);
});