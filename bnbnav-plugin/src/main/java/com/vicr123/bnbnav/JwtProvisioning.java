package com.vicr123.bnbnav;

import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.exceptions.JWTVerificationException;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

import java.util.Date;

public class JwtProvisioning {
    public static String JwtFor(CommandSender sender) {
        String subject = "bnbnav";
        String pn = "bnbnav";
        if (sender instanceof Player player) {
            subject = player.getUniqueId().toString();
            pn = player.getName();
        }
        Algorithm algorithm = Algorithm.HMAC256(System.getenv("BNBNAV_JWT_TOKEN"));
        return JWT.create()
                .withIssuer("bnbnav")
                .withSubject(subject)
                .withClaim("pn", pn)
                .withIssuedAt(new Date())
                .sign(algorithm);
    }

    public static boolean isValidJwt(String jwt) {
        try {
            var algorithm = Algorithm.HMAC256(System.getenv("BNBNAV_JWT_TOKEN"));
            JWT.require(algorithm).withIssuer("bnbnav").build().verify(jwt);
            return true;
        } catch (JWTVerificationException decodeException) {
            return false;
        }
    }
}
