package com.vicr123.bnbnav.commands;

import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import net.md_5.bungee.api.ChatColor;
import net.md_5.bungee.api.chat.ClickEvent;
import net.md_5.bungee.api.chat.TextComponent;
import org.bukkit.command.Command;
import org.bukkit.command.CommandExecutor;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

import java.util.Calendar;
import java.util.Date;

public class EditNavCommand implements CommandExecutor {
    @Override
    public boolean onCommand(CommandSender commandSender, Command command, String s, String[] strings) {
        if (!(commandSender instanceof Player)) {
            commandSender.sendMessage("editnav can only be used with a player.");
            return false;
        }

        if (!commandSender.hasPermission("bnbnav.edit")) {
            commandSender.sendMessage("You do not have permission to edit bnbnav.");
            return false;
        }

        Algorithm algorithm = Algorithm.HMAC256(System.getenv("BNBNAV_JWT_TOKEN"));
        String token = JWT.create()
                .withIssuer("bnbnav")
                .withSubject(((Player) commandSender).getUniqueId().toString())
                .withClaim("pn", commandSender.getName())
                .withIssuedAt(new Date())
                .sign(algorithm);

        TextComponent component = new TextComponent("Click to type your token in chat.");
        component.setColor(ChatColor.GREEN);
        component.setUnderlined(true);
        component.setClickEvent(new ClickEvent(ClickEvent.Action.SUGGEST_COMMAND, token));
        commandSender.spigot().sendMessage(component);

        return false;
    }
}
