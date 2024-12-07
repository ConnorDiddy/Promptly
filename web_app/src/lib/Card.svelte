<script>
  import { isConnected, myCharacteristic, notifications } from "../stores.js";

  var serviceId = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  var characteristicId = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

  async function enableNotifications(characteristic) {
    console.log(characteristic);
    if (characteristic.properties.notify) {
      characteristic.addEventListener("characteristicvaluechanged", (event) => {
        const value = new TextDecoder().decode(event.target.value);
        console.log("Received value:", value);
        const [char, id] = value.split(":");
        notifications.update((n) => {
          n[id] = char;
          return n;
        });
      });

      await characteristic.startNotifications();
      console.log("Notifications started!");
    } else {
      console.log("Notifications are not supported on this characteristic.");
    }
  }

  async function handleBluetoothConnect() {
    try {
      // @ts-ignore
      navigator.bluetooth
        .requestDevice({
          filters: [{ services: [serviceId] }],
        })
        .then((device) => {
          return device.gatt.connect();
        })
        .then((server) => {
          return server.getPrimaryService(serviceId);
        })
        .then((service) => {
          return service.getCharacteristic(characteristicId);
        })
        .then((characteristic) => {
          myCharacteristic.set(characteristic);
          isConnected.set(true);
          enableNotifications(characteristic);
        });
    } catch (error) {
      console.error("Error", error);
    }
  }

  async function handleBluetoothDisconnect() {
    window.location.reload();
  }
</script>

<main class="container">
  <div class="card">
    <div id="connect-button-row">
      <button
        class={$isConnected ? "connected" : ""}
        on:click={$isConnected
          ? handleBluetoothDisconnect
          : handleBluetoothConnect}
      >
        {$isConnected ? "Disconnect" : "Connect"}
      </button>
    </div>
    <slot />
  </div>
</main>

<style>
  .container {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100vw;
    height: 100vh;
    margin: 0;
  }

  .card {
    background-color: white;
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 12px;
    border-radius: 8px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    min-width: 500px;
  }

  #connect-button-row {
    display: flex;
    justify-content: end;
    margin-bottom: 16px;
    width: 100%;
  }

  button {
    border-radius: 9999px;
    padding-inline: 16px;
    padding-block: 8px;
    border: none;
    box-shadow: none;
  }
</style>
