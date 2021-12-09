import mongoUtil from "../utils/database.js";
import { Constants } from "../constants.js";

/**
 * This script defines our mongodb interaction.
 */
const checkEmployee = (req, res, next) => {
  if (typeof req.query.uid === "undefined") {
    return res
      .status(Constants.responseHTTP.badRequest)
      .json({ message: Constants.responseMsg.invalidParams });
  }
  console.log(`[GET] /authorize (uid: ${req.query.uid})`);

  var db = mongoUtil.getDb();
  var userCollections = db.collection(Constants.database.employeeCollections);
  userCollections
    .findOne({
      uid: req.query.uid,
    })
    .then((dbUser) => {
      // if uid doesnt exist
      if (!dbUser) {
        return res
          .status(Constants.responseHTTP.unaothorized)
          .json({ message: Constants.responseMsg.invalidUser });
      }

      return res.status(Constants.responseHTTP.success).json({
        message: Constants.responseMsg.authorizedMessage,
        name: dbUser.name,
        uid: dbUser.uid,
      });
    });
};
export default checkEmployee;
