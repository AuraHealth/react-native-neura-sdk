package com.neurareact.neurasdkmanager;

import android.os.Message;
import android.util.Log;
import android.os.Bundle;

import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.Arguments;

import com.neura.standalonesdk.service.NeuraApiClient;
import com.neura.standalonesdk.util.Builder;
import com.neura.standalonesdk.util.SDKUtils;

import com.neura.standalonesdk.events.NeuraEventCallBack;
import com.neura.resources.authentication.AnonymousAuthenticateCallBack;
import com.neura.resources.authentication.AnonymousAuthenticateData;
import com.neura.sdk.object.AnonymousAuthenticationRequest;
import com.neura.sdk.service.SubscriptionRequestCallbacks;
import com.neura.sdk.object.EventDefinition;
import com.google.firebase.iid.FirebaseInstanceId;
import com.neura.resources.user.UserDetailsCallbacks;
import com.neura.resources.user.UserDetails;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class NeuraSDKModule extends ReactContextBaseJavaModule {
  private NeuraApiClient mNeuraApiClient;

  private Callback success;
  private Callback error;


  private String appUid = "";
  private String appSecret = "";

  public NeuraSDKModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  public String getName() {
    return "Neura";
  }

  @ReactMethod
  public void init(String appUid, String appSecret, Promise promise) {
    Builder builder = new Builder(getReactApplicationContext());
    mNeuraApiClient = NeuraApiClient.getClient(getReactApplicationContext(), appUid, appSecret);
    mNeuraApiClient.connect();
    promise.resolve("initialized");
  }

  @ReactMethod
  public void authenticate(final Promise promise) {

    if (mNeuraApiClient != null) {
      String pushToken = FirebaseInstanceId.getInstance().getToken();

      AnonymousAuthenticationRequest request = new AnonymousAuthenticationRequest(pushToken);

      mNeuraApiClient.authenticate(request, new AnonymousAuthenticateCallBack() {
        @Override
        public void onSuccess(AnonymousAuthenticateData authenticateData) {
          Log.i(getClass().getSimpleName(), "Successfully requested authentication with neura. ");
          promise.resolve(authenticateData);
        }

        @Override
        public void onFailure(int errorCode) {
          Log.e(getClass().getSimpleName(), "Failed to authenticate with neura. "
                  + "Reason : " + SDKUtils.errorCodeToString(errorCode));
          promise.resolve(SDKUtils.errorCodeToString(errorCode));
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  @ReactMethod
  public void isAuthenticated(Promise promise) {
    if (mNeuraApiClient != null) {
      Boolean isLoggedIn = mNeuraApiClient.isLoggedIn();
      promise.resolve(isLoggedIn);
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  @ReactMethod
  public void getUserAccessToken(Promise promise) {
    if (mNeuraApiClient != null) {
      String token = mNeuraApiClient.getUserAccessToken();
      promise.resolve(token);
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  @ReactMethod
  public void getUserId(final Promise promise) {
    if (mNeuraApiClient != null) {
      mNeuraApiClient.getUserDetails(new UserDetailsCallbacks() {
        @Override
        public void onSuccess(UserDetails userDetails) {
          promise.resolve(userDetails);
        }

        @Override
        public void onFailure(Bundle resultData, int errorCode) {
          promise.reject(new Error());
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  @ReactMethod
  public void notificationHandler(ReadableMap details) {
    Bundle bundle = Arguments.toBundle(details);

    boolean isNeuraPush = NeuraPushCommandFactory.getInstance().isNeuraPush(getApplicationContext(), bundle, new NeuraEventCallBack() {
      @Override
      public void neuraEventDetected(NeuraEvent event) {
          Log.d("Neura event:", event);
          promise.resolve(event);
      }
    });
    if(!isNeuraPush) {
      promise.reject(new Error("Neura event not found"));
    }
  }

  @ReactMethod
  public void logOut(final Promise promise) {
    if (mNeuraApiClient != null) {
      mNeuraApiClient.forgetMe(new android.os.Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
          if (msg.arg1 == 1) {
            // check if forget me was successful using isLoggedIn
            if (mNeuraApiClient.isLoggedIn()) {
              promise.resolve(true);
            } else {
              promise.resolve(false);
            }
          }
          promise.resolve(false);
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  // follow https://dev.theneura.com/api-reference/situations-and-moments/ For possible events you can pass as event parameter.
  @ReactMethod
  public void simulateEvent(String event, final Promise promise) {
    if (mNeuraApiClient != null) {
      mNeuraApiClient.simulateEvent(eventName, new SimulateEventCallback() {
        @Override
        public void onSuccess(String eventName) {
          Log.i(getClass().getSimpleName(), "Successfully simulated the event: " + eventName);
          promise.resolve(true);
        }

        @Override
        public void onFailure(String eventName, String message) {
          Log.e(getClass().getSimpleName(), "Failed to simulate the event: " + eventName
                  + ", Reason : " + message);
          promise.resolve(message);
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  // follow https://dev.theneura.com/pages/how-to-use-engagement-api/ for details on how to utilize the Insights API
  @ReactMethod
  public void tagEngagementAttempt(String featureName, String instanceId, String value, final Promise promise) {
    int result = NeuraEngagements.tagEngagementAttempt(getApplicationContext(), featureName, instanceId, value);
    if (result == SUCCESS) {
      promise.resolve(true);
    } else {
      promise.reject(new Error(result));
    }
  }

  // follow https://dev.theneura.com/pages/how-to-use-engagement-api/ for details on how to utilize the Insights API
  @ReactMethod
  public void tagEngagementFeature(String featureName, String instanceId, String action, String value, final Promise promise) {
    int result = NeuraEngagements.tagEngagementFeature(getApplicationContext(), featureName, instanceId, action, value);
    if (result == SUCCESS) {
      promise.resolve(true);
    } else {
      promise.reject(new Error(result));
    }
  }

  @ReactMethod
  public void addWebHookSubscription(String event, String eventIdentifier, String webhookId, final Promise promise) {
    if (mNeuraApiClient != null) {
      mNeuraApiClient.subscribeToEvent(event, eventIdentifier, SubscriptionMethod.WEBHOOK, webhookId, new SubscriptionRequestCallbacks() {
        @Override
        public void onSuccess(String eventName, Bundle resultData, String identifier) {
          promise.resolve(true);
        }

        @Override
        public void onFailure(String eventName, Bundle resultData, int errorCode) {
          promise.reject(new Error(errorCode));
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }

  @ReactMethod
  public void addPushSubscription(String event, String eventIdentifier, final Promise promise) {
    if (mNeuraApiClient != null) {
      mNeuraApiClient.subscribeToEvent(event, eventIdentifier, new SubscriptionRequestCallbacks() {
        @Override
        public void onSuccess(String eventName, Bundle resultData, String identifier) {
          promise.resolve(true);
        }

        @Override
        public void onFailure(String eventName, Bundle resultData, int errorCode) {
          promise.reject(new Error(errorCode));
        }
      });
    } else {
      promise.reoslve("init() must be called first");
    }
  }
}