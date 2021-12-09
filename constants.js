const Constants = {
  database: {
    employeeCollections: "employees",
  },
  responseHTTP: {
    success: 200,
    badRequest: 400,
    unaothorized: 401,
    notFound: 404,
    conflict: 409,
    internalServerError: 500,
    badGateway: 502,
  },
  responseMsg: {
    invalidParams:
      "Invalid parameters! Make sure to include `uid` as a parameter.",
    invalidUser: "Unauthorized UID credential.",
    authorizedMessage: "Succesfully authenticated!",
  },
};

export { Constants };
