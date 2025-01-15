#!/usr/bin/env groovy

/* SPDX-License-Identifier: MIT-0 */

def examples = ['drake_bazel_external', 'drake_bazel_external_legacy', 'drake_cmake_external']
def jobs = [:]

for (x in examples) {
    def example = x

    jobs[example] = {
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
              }
              dir('src/' + example){
                stage(example + ' setup') {
                  sh '.github/setup'
                }
                stage(example + ' build and test') {
                  sh '.github/ci_build_test'
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
    }
}

parallel jobs
