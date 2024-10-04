import {ClusterName, Clusters, CommandGeneral} from '../../protocol/Cluster.js'
import {Message} from '../../protocol/Message.js'

Object.defineProperty(String.prototype, 'capitalize', {
    value: function () {
        return this.charAt(0).toUpperCase() + this.slice(1).toLowerCase();
    },
    enumerable: false
});

export class Treeview {
    constructor(messageManager) {
        this.treeviewList = $('#treeview-list');
        this.initMessageManagerCallbacks(messageManager);
        this.showTreeview();
    }

    initMessageManagerCallbacks(messageManager) {
        this.messageManager = messageManager;
        this.messageManager.addCallbackRead(this.getMessage);
    }

    getMessage(message) {
        if (ClusterName.GENERAL === message.cluster.name) {
            if (CommandGeneral.VERSION === message.command.name) {
                let major = message.fetchInt8U();
                let minor = message.fetchInt8U();
                $('#treeview-list-cluster-command-value-' + message.cluster.name + "-" + message.command.name).text(major + '.' + minor);
            }
        }
    }

    addClusterListToTreeview() {
        for (let index = 0; index < Clusters.clusters.length; index++) {
            const cluster = Clusters.clusters[index];

            let itemCluster = document.createElement("li");
            itemCluster.className = "list-group-item";

            let itemClusterContent = document.createElement("div");
            let itemClusterContentdiv = document.createElement("div");
            itemClusterContentdiv.className = "d-inline-flex"

            let itemClusterImage = document.createElement("i");
            itemClusterImage.className = "bi bi-plus me-2"

            let itemClusterButton = document.createElement("div");
            itemClusterButton.innerText = cluster.name.capitalize();

            itemClusterButton.setAttribute('type', 'button');
            itemClusterButton.setAttribute('data-bs-toggle', 'collapse');
            itemClusterButton.setAttribute('data-bs-target', '#treeview-list-cluster-' + cluster.name);
            itemClusterButton.setAttribute('aria-expanded', 'false');

            itemClusterContentdiv.appendChild(itemClusterImage);
            itemClusterContentdiv.appendChild(itemClusterButton);
            itemClusterContent.appendChild(itemClusterContentdiv);
            itemClusterContent.appendChild(this.addCommandListToTreeview(cluster.name, Clusters.clusters[index].commands));
            itemCluster.appendChild(itemClusterContent);
            this.treeviewList.append(itemCluster);
        }
    }

    addCommandListToTreeview(clusterName, commands) {
        let itemCluster = document.createElement("ul");
        itemCluster.className = "list-group list-group-flush collapse";
        itemCluster.id = 'treeview-list-cluster-' + clusterName;

        for (let index = 0; index < commands.length - 1; index++) {
            const command = commands[index];

            let itemCommand = document.createElement("li");
            itemCommand.className = "list-group-item p-0 ps-3";

            let itemCommandContentdiv = document.createElement("div");
            itemCommandContentdiv.className = "d-flex justify-content-between align-items-center w-100"

            let itemCommandButton = document.createElement("div");
            itemCommandButton.innerText = command.name.capitalize();

            let itemCommandRightSide = document.createElement("div");
            itemCommandRightSide.className = "d-inline-flex align-items-center"
            let itemCommandValue = document.createElement("span");
            itemCommandValue.className = "text-success"
            itemCommandValue.id = 'treeview-list-cluster-command-value-' + clusterName + "-" + command.name;
            itemCommandValue.innerText = "0"

            let itemCommandUpdateButton = document.createElement("button");
            itemCommandUpdateButton.className = "btn btn-secondary ms-3 btn-sm"
            itemCommandUpdateButton.id = 'treeview-list-cluster-command-update-' + clusterName + "-" + command.name;
            itemCommandUpdateButton.setAttribute('cluster-name', clusterName);
            itemCommandUpdateButton.setAttribute('command-name', command.name);
            let itemCommandUpdateButtonIcon = document.createElement("i");
            itemCommandUpdateButtonIcon.className = "bi bi-arrow-clockwise"

            function handleClick() {
                let cluster = itemCommandUpdateButton.getAttribute("cluster-name");
                let command = itemCommandUpdateButton.getAttribute("command-name");
                this.messageManager.write(new Message().build("Tx", cluster, command));
            }

            itemCommandUpdateButton.addEventListener('click', handleClick.bind(this), false)

            itemCommandContentdiv.appendChild(itemCommandButton);
            itemCommandRightSide.appendChild(itemCommandValue);
            itemCommandUpdateButton.appendChild(itemCommandUpdateButtonIcon);
            itemCommandRightSide.appendChild(itemCommandUpdateButton);
            itemCommandContentdiv.appendChild(itemCommandRightSide);
            itemCommand.appendChild(itemCommandContentdiv);
            itemCluster.appendChild(itemCommand);
        }
        return itemCluster;
    }

    showTreeview() {
        this.addClusterListToTreeview();
    }
}