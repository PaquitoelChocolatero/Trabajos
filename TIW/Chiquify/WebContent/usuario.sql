CREATE TABLE `usuario` (
  `nombre` varchar(50) NOT NULL,
  `apellido1` varchar(45) NOT NULL,
  `apellido2` varchar(45) DEFAULT NULL,
  `ciudad` varchar(45) NOT NULL,
  `email` varchar(45) NOT NULL,
  `passwd` varchar(255) NOT NULL,
  PRIMARY KEY (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Tabla que contiene la información de todos los usuarios registrados en la aplicación'