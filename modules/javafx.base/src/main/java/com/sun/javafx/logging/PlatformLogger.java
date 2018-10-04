/*
 * Copyright (c) 2009, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package com.sun.javafx.logging;

import java.util.logging.Logger;
import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

/**
 * Platform logger provides an API for the JavaFX components to log
 * messages. It is an internal logger intended to be used by JavaFX modules.
 *
 * The PlatformLogger uses an underlying System.Logger
 * obtained by calling {@link java.lang.System.Logger#getLogger(String)}
 *
 * PlatformLogger implements System.Logger as to facilitiate logging of
 * calling class and method.
 * Note : JDK internal loggers know to skip any calls from System.Logger
 * classes when looking for the calling class / method.
 */
public class PlatformLogger { //implements System.Logger {

    /**
     * PlatformLogger logging levels.
     */
    public static enum Level {
        // The name and value must match that of {@code java.lang.System.Level}s.
        // Declare in ascending order of the given value
        ALL(java.util.logging.Level.ALL),
        FINEST(java.util.logging.Level.FINEST),
        FINER(java.util.logging.Level.FINER),
        FINE(java.util.logging.Level.FINE),
        INFO(java.util.logging.Level.INFO),
        WARNING(java.util.logging.Level.WARNING),
        SEVERE(java.util.logging.Level.SEVERE),
        OFF(java.util.logging.Level.OFF);

        final java.util.logging.Level systemLevel;
        Level(java.util.logging.Level systemLevel) {
            this.systemLevel = systemLevel;
        }
    }

    private java.util.logging.Level getSystemLoggerLevel(Level l) {
        switch (l) {
            case ALL :    return java.util.logging.Level.ALL;
            case FINEST:  return java.util.logging.Level.FINEST;
            case FINER:   return java.util.logging.Level.FINER;
            case FINE:    return java.util.logging.Level.FINE;
            case INFO:    return java.util.logging.Level.INFO;
            case WARNING: return java.util.logging.Level.WARNING;
            case SEVERE:  return java.util.logging.Level.SEVERE;
            case OFF:     return java.util.logging.Level.OFF;
            default :     return java.util.logging.Level.ALL;
        }
    }


    // Table of known loggers.  Maps names to PlatformLoggers.
    private static final Map<String,WeakReference<PlatformLogger>> loggers =
        new HashMap<>();

    /**
     * Returns a PlatformLogger of a given name.
     * @param name the name of the logger
     * @return a PlatformLogger
     */
    public static synchronized PlatformLogger getLogger(String name) {
        PlatformLogger log = null;
        WeakReference<PlatformLogger> ref = loggers.get(name);
        if (ref != null) {
            log = ref.get();
        }
        if (log == null) {
            log = new PlatformLogger(Logger.getLogger(name));
            loggers.put(name, new WeakReference<>(log));
        }
        return log;
    }

    public String getName() {
        return this.loggerProxy.getName();
    }


    private final Logger loggerProxy;
    protected PlatformLogger(java.util.logging.Logger loggerProxy) {
        this.loggerProxy = loggerProxy;
    }

    // ------------------------------------------------------------------------
    //          From System.Logger interface
    // ------------------------------------------------------------------------

    /**
     * Gets the name for this platform logger.
     * @return the name of the platform logger.
    @Override
    public String getName() {
       throw new UnsupportedOperationException("not implemented");
    }

    @Override
    public boolean isLoggable(Level level) {
        throw new UnsupportedOperationException("not implemented");
    }

    @Override
    public void log(Level level, ResourceBundle bundle, String format, Object... params) {
        throw new UnsupportedOperationException("not implemented");
    }

    @Override
    public void log(Level level, ResourceBundle bundle, String msg, Throwable thrown) {
        throw new UnsupportedOperationException("not implemented");
    }
*/

    // ------------------------------------------------------------------------


    /**
     * Returns true if a message of the given level would actually
     * be logged by this logger.
     * @param level the level
     * @return whether a message of that level would be logged
     */
    public boolean isLoggable(Level level) {
        if (level == null) {
            throw new NullPointerException();
        }

        return loggerProxy.isLoggable(getSystemLoggerLevel(level));
    }

    /**
     * Logs a SEVERE message.
     * @param msg the message
     */
    public void severe(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.SEVERE, msg, (Object[])null);
    }

    public void severe(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.SEVERE, msg, t);
    }

    public void severe(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.SEVERE, msg, params);
    }

    /**
     * Logs a WARNING message.
     * @param msg the message
     */
    public void warning(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.WARNING, msg, (Object[])null);
    }

    public void warning(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.WARNING, msg, t);
    }

    public void warning(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.WARNING, msg, params);
    }

    /**
     * Logs an INFO message.
     * @param msg the message
     */
    public void info(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.INFO, msg, (Object[])null);
    }

    public void info(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.INFO, msg, t);
    }

    public void info(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.INFO, msg, params);
    }

    /**
     * Logs a FINE message.
     * @param msg the message
     */
    public void fine(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINE, msg, (Object[])null);
    }

    public void fine(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINE, msg, t);
    }

    public void fine(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINE, msg, params);
    }

    /**
     * Logs a FINER message.
     * @param msg the message
     */
    public void finer(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINER, msg, (Object[])null);
    }

    public void finer(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINER, msg, t);
    }

    public void finer(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINER, msg, params);
    }

    /**
     * Logs a FINEST message.
     * @param msg the message
     */
    public void finest(String msg) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINEST, msg, (Object[])null);
    }

    public void finest(String msg, Throwable t) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINEST, msg, t);
    }

    public void finest(String msg, Object... params) {
        if (!loggingEnabled) return;
        loggerProxy.log(java.util.logging.Level.FINEST, msg, params);
    }

    // Methods for unit tests
    private boolean loggingEnabled = true;
    public void enableLogging() {
        loggingEnabled = true;
    };

    public void disableLogging() {
        loggingEnabled = false;
    }
}

