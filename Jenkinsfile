#!/usr/bin/env groovy

/* SPDX-License-Identifier: MIT-0 */

node('linux-jammy-unprovisioned') {
  timeout(600) {
    ansiColor('xterm') {
      def triggers = []
      if (env.BRANCH_NAME == 'main') {
        triggers << cron('H H(7-8) * * *')
      }
      properties ([
        pipelineTriggers(triggers)
      ])
      try {
        dir('src') {
          stage('checkout') {
            checkout scm
          }
          stage('setup') {
            sh './scripts/continuous_integration/jenkins/setup'
          }
          stage('build and test') {
            sh './scripts/continuous_integration/jenkins/build_test'
          }
        }
      } catch (e) {
        if (env.BRANCH_NAME == 'main') {
          emailext (
            subject: "Build failed in Jenkins: ${env.JOB_NAME} #${env.BUILD_NUMBER}",
            body: "See <${env.BUILD_URL}display/redirect?page=changes>",
            recipientProviders: [[$class: 'DevelopersRecipientProvider']],
            to: '$DEFAULT_RECIPIENTS',
          )
        }
        throw e
      } finally {
        deleteDir()
        dir("${env.WORKSPACE}@tmp") {
          deleteDir()
        }
      }
    }
  }
}
