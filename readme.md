# SmartGate Arduino IoT

This repository contains the source code of our SmartGate Arduino IoT.

## Project Setup

### Backend Systems

This project uses `mongodb` as the cloud service to store user credentials. Our backend system runs on `express-js` to connect to the database & to manage the authentication logic.

#### MongoDB Setup

You can follow the tutorial to setup the MongoDB Atlas cluster [here](https://www.mongodb.com/basics/clusters/mongodb-cluster-setup). The data structure for our `user` object is as follows:

```
User {
    name: String,
    uid: String,
}
```

Keep in mind that MongoDB Atlas will assign `_id` attribute by default.

#### ExpressJS Setup

Navigate to `utils/database.js` and change the credentials (_username, password, dbName, collectionName_) and the URI to appropriately connect to your newly setup MongoDB Atlas cluster.
Run `npm start` to start running the backend service locally. `DEFAULT_PORT` is set to **1880**.

###
