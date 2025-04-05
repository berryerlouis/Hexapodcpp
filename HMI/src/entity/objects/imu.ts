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
            this.imuData.accel.x = this.fetchInt16S(message.params[0], message.params[1]);
            this.imuData.accel.y = this.fetchInt16S(message.params[2], message.params[3]);
            this.imuData.accel.z = this.fetchInt16S(message.params[4], message.params[5]);
            this.imuData.gyro.x = this.fetchInt16S(message.params[6], message.params[7]);
            this.imuData.gyro.y = this.fetchInt16S(message.params[8], message.params[9]);
            this.imuData.gyro.z = this.fetchInt16S(message.params[10], message.params[11]);
            this.imuData.mag.x = this.fetchInt16S(message.params[12], message.params[13]);
            this.imuData.mag.y = this.fetchInt16S(message.params[14], message.params[15]);
            this.imuData.mag.z = this.fetchInt16S(message.params[16], message.params[17]);

        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.ALTITUDE, (message:Message) => {
            this.imuData.altitude = this.fetchInt16S(message.params[0], message.params[1]);
            document.getElementById('altitude')!.innerText = (this.imuData.altitude).toFixed(1);
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.PRESSURE, (message:Message) => {
            this.imuData.pressure = (message.params[3]<<24) + (message.params[2]<<16) + (message.params[1]<<8) + (message.params[0]);
            document.getElementById('pressure')!.innerText = this.imuData.pressure.toString();
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.TMPBAR, (message:Message) => {
            this.imuData.temperature = ((message.params[1]<<8) + (message.params[0]))/100;
            document.getElementById('temperature')!.innerText = (this.imuData.temperature).toFixed(2);
        });
        socket.addSpecificCallbackRead(ClusterName.IMU, ClusterImuCommands.YAWPITCHROLL, (message:Message) => {
            this.imuData.ypr.pitch = this.fetchInt16S(message.params[0], message.params[1])/100;
            this.imuData.ypr.roll  = this.fetchInt16S(message.params[2], message.params[3])/100;
            this.imuData.ypr.yaw = this.fetchInt16S(message.params[4], message.params[5])/100;
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
    fetchInt16U(param0:number, param1:number) {
        return (param0<<8) + param1;
    }

    fetchInt16S(param0:number, param1:number) {
        let num = this.fetchInt16U(param1,param0);
        if (num & 0x8000) {
            num = -(0x10000 - num);
        }
        return num;
    }
}