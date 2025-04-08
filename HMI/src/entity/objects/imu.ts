import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import Socket from "../../communication/socket.ts";
import {ClusterImuCommands} from "../../communication/clusters/clusterImu.ts";

interface ImuStruct {
    accel: { x: number, y: number, z: number };
    gyro: { x: number, y: number, z: number };
    mag: { x: number, y: number, z: number };
    ypr: { yaw: number, pitch: number, roll: number };
    temperature: number;
    altitude: number;
    pressure: number;
}


export default class Imu {

    intervalPrimary: number;
    intervalSecondary: number;
    socket: Socket;
    imuData: ImuStruct = {
        accel: {x: 0, y: 0, z: 0},
        gyro: {x: 0, y: 0, z: 0},
        mag: {x: 0, y: 0, z: 0},
        ypr: { yaw: 0, pitch: 0, roll: 0 },
        temperature: 0,
        altitude : 0,
        pressure : 0
    }
    constructor(socket: Socket, intervalCommand:number) {
        this.socket = socket;
        this.intervalPrimary = 0;
        this.intervalSecondary = 0;

        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.ALL, (message:Message) => {
            this.imuData.accel.x = message.getValueInt16(0);
            this.imuData.accel.y = message.getValueInt16(2);
            this.imuData.accel.z = message.getValueInt16(4);
            this.imuData.gyro.x = message.getValueInt16(6);
            this.imuData.gyro.y = message.getValueInt16(8);
            this.imuData.gyro.z = message.getValueInt16(10);
            this.imuData.mag.x = message.getValueInt16(12);
            this.imuData.mag.y = message.getValueInt16(14);
            this.imuData.mag.z = message.getValueInt16(16);
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.ALTITUDE, (message:Message) => {
            this.imuData.altitude = message.getValueInt16(0);
            document.getElementById('altitude')!.innerText = (this.imuData.altitude).toFixed(1);
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.PRESSURE, (message:Message) => {
            this.imuData.pressure = message.getValueUint32(0);
            document.getElementById('pressure')!.innerText = this.imuData.pressure.toString();
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.TMPBAR, (message:Message) => {
            this.imuData.temperature = (message.getValueUint16(0))/100;
            document.getElementById('temperature')!.innerText = (this.imuData.temperature).toFixed(2);
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.YAWPITCHROLL, (message:Message) => {
            this.imuData.ypr.pitch = message.getValueInt16(0)/100;
            this.imuData.ypr.roll  = message.getValueInt16(2)/100;
            this.imuData.ypr.yaw = message.getValueInt16(4)/100;
            document.getElementById('yaw')!.innerText = (this.imuData.ypr.yaw).toFixed(2);
            document.getElementById('pitch')!.innerText = (this.imuData.ypr.pitch).toFixed(2);
            document.getElementById('roll')!.innerText = (this.imuData.ypr.roll).toFixed(2);
        });

        this.socket.addCallbackStopped(()=> {
            clearInterval(this.intervalPrimary);
            clearInterval(this.intervalSecondary);
        });

        this.socket.addCallbackStarted(()=> {
            this.intervalPrimary = setInterval(()=>{
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.ALTITUDE));
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.PRESSURE));
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.TMPBAR));
            },intervalCommand);

            this.intervalSecondary = setInterval(()=>{
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.ALL));
                this.socket.write(new Message( ClusterName.IMU, ClusterImuCommands.YAWPITCHROLL));
            },intervalCommand);
        });
    }
}