CREATE TABLE `producto` (
  `idproduct` int NOT NULL AUTO_INCREMENT,
  `titulo` varchar(45) NOT NULL,
  `categoria` varchar(45) DEFAULT NULL,
  `descripcion` varchar(255) DEFAULT NULL,
  `imagen` varchar(45) NOT NULL,
  `precio` int NOT NULL,
  `vendedor` varchar(45) NOT NULL,
  `comprador` varchar(45) DEFAULT NULL,
  `estado` varchar(45) NOT NULL DEFAULT 'Disponible',
  PRIMARY KEY (`idproduct`),
  KEY `vendedor_idx` (`vendedor`),
  KEY `comprador_idx` (`comprador`),
  CONSTRAINT `comprador` FOREIGN KEY (`comprador`) REFERENCES `user` (`email`) ON UPDATE CASCADE,
  CONSTRAINT `vendedor` FOREIGN KEY (`vendedor`) REFERENCES `user` (`email`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Productos añadidos por los usuarios en la aplicación'