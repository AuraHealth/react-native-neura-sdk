/**
 *  NeuraSDK.h
 *  NeuraSDK
 *
 *  Created by Gal Mirkin on 8/20/15.
 *  Copyright (c) 2015 Neura. All rights reserved.
 */

#import <UIKit/UIKit.h>
#import "NeuraSdkConfiguration.h"
#import "NeuraSDKAPIAvailability.h"
#import "NeuraSDKAPIResults.h"
#import "NeuraSDKAPIRequests.h"
#import "NeuraSDKPushNotification.h"
#import "NeuraAuthenticationStateDelegate.h"
#import "NSubscriptionStatus.h"
#import "NUserEngagementAttributes.h"
#import "NEvent+Action.h"
#import "NEvent+Feedback.h"

#pragma mark - New Public API
/**
NeuraSDK API Version 4.9
*/
@interface NeuraSDK: NSObject

#pragma mark -- Shared Instance
/** @name Shared Instance */

#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
/** shared class property
 
 The same as [NeuraSDK sharedInstance].
 This is just a convenience class property for shorter syntax under Swift 3.0+
 
 Starting Swift 3.0+
 Instead of : NeuraSDK.sharedInstance()?.someMethodName()
 You can just use: NeuraSDK.shared.someMethodName()
 */
@property (class, nonatomic, readonly, nonnull) NeuraSDK *shared;
#endif

/** The shared instance of the SDK.
    Must be placed in the AppDelegate on applicationDidFinishLaunchingWithOptions */
+ (nullable NeuraSDK *)sharedInstance;

#pragma mark -- Application identifiers
/** @name Application identifiers */

/** the App UID from the Neura dev site */
@property (atomic, strong, nullable, readonly) NSString *appUID;

/** the App Secret from the Neura dev site */
@property (atomic, strong, nullable, readonly) NSString *appSecret;

/**
 Use to Identification the app.
 
 @param appUID the App UID from the Neura dev site
 @param appSecret the App Secret from the Neura dev site.
 */

-(void)setAppUID:(NSString *)appUID appSecret:(NSString* )appSecret;

#pragma mark - SDK Info
/** @name SDK Info */

/**
 *  get the sdk version
 *
 *  @return version string
 */
- (nullable NSString *)getVersion;

#pragma mark - App Permissions
/** @name App Permissions */

/**
 *  use to get the app permissions list
 *
 *  This call is asynchronous.
 *  The result handler will provide the list of permissions as an array of Permission objects.
 *
 *  @param callback PermissionsListCallback returning the NeuraPermissionsListResult.
 */
- (void)getAppPermissionsListWithCallback:(nonnull NeuraPermissionsListCallback)callback;

#pragma mark - Authentication
/** @name Authentication */

/**
 Get the current authentication state.
 See: NeuraAuthState for all possible values and meaning.

 @return NeuraAuthState of the current authentication state.
 */
- (NeuraAuthState)authenticationState;


/**
 Returns true if the app was already authenticated successfully
 (access token was already received successfully)
 
 @return BOOL - YES if already authenticated and appToken is available. NO otherwise.
 */
- (BOOL)isAuthenticated;

/**
 An app token received on authentication.
 
 @return NSString - the application's token.
 */
- (nullable NSString *)appToken;


/**
 The (API) unique identifier of the Neura user.

 @return A string with the unique Neura identifier for this authenticated user. 
 Will return nil if the user is not signed in / authenticated.
 */
- (nullable NSString *)neuraUserId;

/**
 Use to logout from Neura

 @param callback callbak NeuraLogoutCallback returning the NeuraLogoutResult.
 */
- (void)logoutWithCallback:(nonnull NeuraLogoutCallback)callback;

/**
 Use to authenticate the app.
 There are a few possible authentication flows and configurations.
 @see: NeuraAuthenticationRequest for more details.
 
 @param request (optional) NeuraAuthenticationRequest the authentication configuration object. Pass nil to use the default configuration.
 @param callback NeuraAuthenticateCallback returning a NeuraAuthenticationResult object.
 */
- (void)authenticateWithRequest:(nullable NeuraBaseAuthenticationRequest *)request
                       callback:(nonnull NeuraAuthenticateCallback)callback;

#pragma mark - List of events
/**
 test

 @param callback dsdfgsdfg
 */
- (void)getEventsListWithCallback:(nonnull NeuraEventsListResultCallback)callback;

#pragma mark - Subscriptions (declarative)
/** @name Subscriptions (declarative) */

/**
 This API can be used to declare what events you want your app to be subscribed to and let the
 SDK manage the subscriptions for you.
 Calling this method ( at least once ) will prevent you from calling addSubscription / removeSubscription directly.
 
 @param eventNames A list of event names you want your app to subscribe to.
 @param method Method of the subscription (See NSubscription for more details).
 */
- (void)requireSubscriptionsToEvents:(nonnull NSArray <NSString *> *)eventNames method:(NSubscriptionMethod)method;

/**
 This API can be used to declare what events you want your app to be subscribed to and let the
 SDK manage the subscriptions for you.
 Calling this method ( at least once ) will prevent you from calling addSubscription / removeSubscription directly.
 
 @param eventNames A list of event names you want your app to subscribe to.
 @param method Method of the subscription (See NSubscription for more details).
 @param webhookId (optional) The identifier of the related webhook.
 */
- (void)requireSubscriptionsToEvents:(nonnull NSArray <NSString *> *)eventNames method:(NSubscriptionMethod)method webhookId:(NSString *_Nullable)webhookId;


/**
 Use this to see the status of subscriptions to events you declared the app should subscribe to using the requireSubscriptionsToEvents: method.
 Subsribing to events happens asynchronously in the background by a subscriptions manager of the SDK.
 You can use this method at any time to see the current status of the subscriptions.
 
 @return An array of NSubscriptionStatus objects describing the current status of subscriptions to the events you declared your app require to be subscribed to.
 */
- (NSArray <NSubscriptionStatus *> *_Nonnull)getRequiredSubscriptionsStatus;

/**
 If you called the declarative requireSubscriptionsToEvents method, the SDK will manage subscriptions for you and you'll not be able to manually
 subscribe to events directly.
 Call this method if you want to start handling subscriptions manually using the addSubscription: and removeSubscription: methods.
 Calling this method will cause the SDK to forget the list of moments you declared you are interested to subscribe to using the requireSubscriptionsToEvents method.
 */
- (void)manageSubscriptionsManually;


#pragma mark - Subscriptions
/** @name Subscriptions (explicit) */

/**
 Get a list of current subscriptions.
 
 @param callback NeuraSubscriptionsListCallback returning a NeuraSubscriptionsListResult.
 */
- (void)getSubscriptionsListWithCallback:(nonnull NeuraSubscriptionsListCallback)callback;

/**
 Subscribe to an event using a NSubscription object.
 
 @param subscription NSubscription object. Must provide eventName and identifier. webHookId is optional.
 @param callback NeuraAddSubscriptionCallback - a callback returning the result of this subscribe request.
 */
- (void)addSubscription:(nonnull NSubscription *)subscription
               callback:(nonnull NeuraAddSubscriptionCallback)callback;

/**
 Remove subscription (given a NSubscription object)
 
 @param subscription NSubscription - the subscription object.
 @param callback NeuraRemoveSubscriptionCallback - a callback returning the result of this remove subscription request.
 */
- (void)removeSubscription:(nonnull NSubscription *)subscription
                  callback:(nonnull NeuraRemoveSubscriptionCallback)callback;

/**
 Remove subscription (given a subscription identifier)
 
 @param identifier NSString - the unique subscription identifier.
 @param callback NeuraRemoveSubscriptionCallback - a callback returning the result of this remove subscription request.
 */
- (void)removeSubscriptionWithIdentifier:(nonnull NSString *)identifier
                                callback:(nonnull NeuraRemoveSubscriptionCallback)callback;

#pragma mark - Feedback
/** @name Feedback */


/**
 Deprecated method.

 Please use sendFeedbackForEventId:feedback:callback
 
 @param neuraID neuraID
 @param approved BOOL approved or not
 @param callback NeuraEventFeedbackCallback callback
 */
- (void)sendFeedbackForEventId:(nonnull NSString *)neuraID
                      approved:(BOOL)approved
                      callback:(nonnull NeuraEventFeedbackCallback)callback DEPRECATED_NEURA_API("Deprecated. Please use sendFeedbackForEventId:feedback:callback");


/**
 Send feedback for an event.

 @param neuraID The id of the event.
 @param feedback NFeedbackType the type of the feedback (positive, negative, ack)
 @param callback NeuraEventFeedbackCallback callback
 */
- (void)sendFeedbackForEventId:(nonnull NSString *)neuraID
                      feedback:(NFeedbackType)feedback
                      callback:(nonnull NeuraEventFeedbackCallback)callback;


#pragma mark - Missing Data
/** @name Missing Data */

/**
 *  Use to check if the event has all the required data
 *
 *  @param eventName event to check if all the require fields exists.
 *
 *  @return false if the event has all the require data in it
 */
- (BOOL)isMissingDataForEvent:(nullable NSString *)eventName;

#pragma mark - Devices & Capabilities
/** @name Devices, User & Capabilities */

/**
 *  get all the supported devices Neura has to offer.
 *
 * @param callback NeuraSupportedDevicesListCallback - a callback returning the NeuraSupportedDevicesListResult.
 */
- (void)getSupportedDevicesListWithCallback:(nonnull NeuraSupportedDevicesListCallback)callback;


/**
 * get all the supported capabilities for devices.
 *
 * @param callback NeuraSupportedCapabilitiesListCallback - a callback returning the NeuraSupportedCapabilitiesListResult.
 */
- (void)getSupportedCapabilitiesListWithCallback:(nonnull NeuraSupportedCapabilitiesListCallback)callback;


/**
 *  check if user has a device with the provided capability.
 *
 *  @param capability NCapability object as can be fetched from (getSupportedCapabilitiesListWithCallback)
 *  @param callback NeuraHasDeviceWithCapabilityCallback - a callback returning the NeuraHasDeviceWithCapabilityResult.
 */
- (void)hasDeviceWithCapability:(nullable NCapability *)capability
                   withCallback:(nullable NeuraHasDeviceWithCapabilityCallback)callback;


/**
 *  Add a device from a list, selected by the user.
 *
 *  @param callback NeuraAddDeviceCallback - a callback returning the NeuraAddDeviceResult.
 */
- (void)addDeviceWithCallback:(nonnull NeuraAddDeviceCallback)callback;

/**
 *  Add a device with the provided name.
 *
 *  @param deviceName The name of the device type to add.
 *  @param callback NeuraAddDeviceCallback - a callback returning the NeuraAddDeviceResult.
 */
- (void)addDeviceNamed:(nonnull NSString *)deviceName
          withCallback:(nonnull NeuraAddDeviceCallback)callback;


/**
 *  Add a device that has the named capability.
 *  Will show a user interface with a list of all devices that have such a capability, allowing user to choose a device to add.
 *
 *  @param capabilityName A name of a capability
 *  @param callback NeuraAddDeviceCallback - a callback returning the NeuraAddDeviceResult.
 */
- (void)addDeviceWithCapabilityNamed:(nonnull NSString *)capabilityName
                        withCallback:(nonnull NeuraAddDeviceCallback)callback;

/**
 Add a place with the provided properties.

 @param place NPlace object. Must provide label. latitude, longitude, address and name are optional
 @param callback NeuraAddPlaceCallback - a callback returning the NeuraAddPlaceResult.
 */
- (void)addPlace:(nonnull NPlace *)place withCallback:(nonnull NeuraAddPlaceCallback)callback;

/**
 Remove a place with the neuraId of a place.
 It is only possible to remove place nodes that were added using the addPlace:withCallback: API.
 On attempt to remove nodes not added with the addPlace:withCallback: API, an error is returned.

 @param neuraId NSString the identifier of the place node to remove.
 @param callback NeuraRemovePlaceCallback - a callback returning the NeuraRemovePlaceResult.
 */
- (void)removePlace:(nonnull NSString *)neuraId withCallback:(nonnull NeuraRemovePlaceCallback)callback;
 

#pragma mark - User
/** @name User */

/**
 Get a collection of states for a user at a given timestamp.
 @see For more info see [SDK docs - Insights](https://dev.theneura.com/docs/api/insights#situation)
 
 @param timeStamp Required. The timestamp for when you want to get the user's situation
 @param contextual In order to get more data about the situation, like previous and next situations. Default: false
 @param callback NeuraGetUserSituationCallback - a callback returning the NeuraGetUserSituationResult
 */
- (void)getUserSituationForTimeStamp:(nonnull NSDate *)timeStamp
                          contextual:(BOOL)contextual
                            callback:(nonnull NeuraGetUserSituationCallback)callback;

/**
 Get the user's wellness information for a single day.
 @see For more info see [SDK docs - Insights](https://dev.theneura.com/docs/api/insights#summary)

 @param date Required. The date for the day you'll get the daily summary for
 @param callback callback NeuraGetInsightInfoCallback - a callback returning the NeuraGetInsightInfoResult
 */
- (void)getSemanticDaySummaryForDate:(nonnull NSDate *)date
                            callback:(nonnull NeuraGetInsightInfoCallback)callback;

/**
 Get the user's sleep information during a period of time.
 @see For more info see [SDK docs - Insights](https://dev.theneura.com/docs/api/insights#sleep)

 @param startDate Required. Day of beginning of the sleep information
 @param endDate <#endDate description#>
 @param callback callback NeuraGetInsightInfoCallback - a callback returning the NeuraGetInsightInfoResult
 */
- (void)getSleepProfileForStartDate:(nonnull NSDate*)startDate
                            endDate:(nonnull NSDate*)endDate
                            callback:(nonnull NeuraGetInsightInfoCallback)callback;

/**
 Get the user's wellness information during a period of time.
 @see For more info see [SDK docs - Insights](https://dev.theneura.com/docs/api/insights#wellness)

 @param startDate Required. Day of beginning of the sleep information
 @param endDate Required. Day of end of the sleep information inclusive
 @param callback callback NeuraGetInsightInfoCallback - a callback returning the NeuraGetInsightInfoResult
 */
- (void)getWellnessLifestyleProfileForStartDate:(nonnull NSDate*)startDate
                                        endDate:(nonnull NSDate*)endDate
                                       callback:(nonnull NeuraGetInsightInfoCallback)callback;

/**
 Get the user's activity information for the place he was at the provided timestamp.
 @see For more info see [SDK docs - Insights](https://dev.theneura.com/docs/api/insights#activity)
 
 @param timeStamp Required. The timestamp for when you want to get the user's activity
 @param resources Required. Array containing the type of data you want to receive. Possible resources are steps and calories. The array Must contain at least one
 @param callback callback NeuraGetInsightInfoCallback - a callback returning the NeuraGetInsightInfoResult
 */
- (void)getActivityProfileAtPlaceForTimeStamp:(nonnull NSDate*)timeStamp
                                    resources:(nonnull NSArray<NSString*> *)resources
                            callback:(nonnull NeuraGetInsightInfoCallback)callback;

/**
 *  Use to open neura settings panel UI.
 *  Shows a list of the permissions that the user approved.
 */
- (void)openNeuraSettingsPanel;

/**
 Simulate an event.
 (sends a fake event, simulating a detection of a Neura event)

 @param name NEventName the name of the event.
 @param callback NeuraAPIResult callback with success/failure.
 */
- (void)simulateEvent:(NEventName)name
             callback:(void (^_Nullable)(NeuraAPIResult * _Nullable result))callback;

#pragma mark - Places services
typedef NS_ENUM(NSUInteger, NeuraPlacesServiceState) {
    NeuraPlacesServicesStateUnavailable,
    NeuraPlacesServiceStateInactive,
    NeuraPlacesServiceStateActive,
    NeuraPlacesServiceStateActiveButPaused
};

/**
 The running state of the integrated places service.

 @return NeuraPlacesServiceState
 */
- (NeuraPlacesServiceState)placesServiceState;

/**
 Call this from your application:performFetchWithCompletionHandler: in app delegate.
 This will allow NeuraSDK to initiate data collection as a background fetch of your app.
 
 The responsibility of calling this method and calling the completion handler with a UIBackgroundFetchResult is left to you the developer,
 in case your app implements its own background fetches functionality.

 @param resultHandler UIBackgroundFetchResult indicating if data collection gathered new data or not.
 */
- (void)collectDataForBGFetchWithResult:(void (^ _Nonnull)(UIBackgroundFetchResult))resultHandler;


#pragma mark - Engagement Api: Events
/** @name Engagement Api */
/**
 This API is aimed to improve user engagement, using it will generate a detailed engagement report that focuses on who the user is, and what the user does.
 For each feature you monitor (up to 10 features), use the attempt as the base call to tag every trigger the feature creates in the app. It can be a notification or in app pop up etc.
 
 The next step is to tag the user’s respone to the engagement attempt. Follow https://dev.theneura.com/pages/how-to-use-engagement-api/ for more information on which action should you use in tagEngagementFeature

 @param featureName Mandatory. The name that will be displayed in the insights dashboard, the feature to monitor. Can contain only alphanumeric (a-z, A-Z, 0-9) and ‘_’, ‘-’ characters
 @param value Optional. Describes specific value of feature. For example: the A/B test is divided to blue and red buttons, use the value to set which group of the test it is
 @param instanceId  Optional. Describes specific instance of engagement incase there is need to bind between several engagement instances.
 @param error validation error if invalid paramaters provided
 @return false if an error occured.
 
 */

- (BOOL)tagEngagementAttempt:(NSString *)featureName
                       value:(nullable NSString *)value
                  instanceId:(nullable NSString *)instanceId
                       error:(NSError **)error;

/**
 This API is aimed to improve user engagement, using it will generate a detailed engagement report that focuses on who the user is, and what the user does.
 Call this method to measure the result of the engagement attempt. For example: if the user converted to a pying customer, use NEngagementFeatureActionSuccess.
 
 Follow https://dev.theneura.com/pages/how-to-use-engagement-api/ for more information on which action should you use in tagEngagementFeature
 @param featureName Mandatory. The name that will be displayed in the insights dashboard, the feature to monitor. Can contain only alphanumeric (a-z, A-Z, 0-9) and ‘_’, ‘-’ characters
 @param action The type  of the action the user performed.
 @param error validation error if invalid paramaters provided
 @return The type  of the action the user performed.
 */
- (BOOL)tagEngagementFeature:(NSString *)featureName
                      action:(NEngagementFeatureAction)action
                       error:(NSError **)error;


/**
 This API is aimed to improve user engagement, using it will generate a detailed engagement report that focuses on who the user is, and what the user does.
 Call this method to measure the result of the engagement attempt. For example: if the user converted to a pying customer, use NEngagementFeatureActionSuccess.
 
 Follow https://dev.theneura.com/pages/how-to-use-engagement-api/ for more information on which action should you use in tagEngagementFeature
 @param featureName Mandatory. The name that will be displayed in the insights dashboard, the feature to monitor. Can contain only alphanumeric (a-z, A-Z, 0-9) and ‘_’, ‘-’ characters
 @param action The type  of the action the user performed.
 @param value Optional. Describes specific value of feature. For example: the A/B test is divided to blue and red buttons, use the value to set which group of the test it is
 @param instanceId Optional. Describes specific instance of engagement incase there is need to bind between several engagement instances.
 @param error validation error if invalid paramaters provided
 @return false if an error occured.
 */
- (BOOL)tagEngagementFeature:(NSString *)featureName
                      action:(NEngagementFeatureAction)action
                       value:(nullable NSString *)value
                  instanceId:(nullable NSString *)instanceId
                       error:(NSError **)error;

/** @name Permissions Api */

/**
 Call this when you want the location authrozation prompt to appear.

 1) Inform the user about the gained value of agreeing to the ALWAYS authorization, using your own UI, before showing the system prompt.
 2) Do this as part of your "user priming" UI flow (on boarding etc) before authenticating to Neura.
 3) The prompt will be shown after Neura authentication, if it wasn't shown by you before that point in time, but for
 getting the highest conversion rates possible, it is recommended to do the above.
 */
- (void)requestAlwaysLocationAuthorization;

/**
 Call this when you want the motion authrozation prompt to appear.
    1) Inform the user about the gained value of agreeing to the prompt, using your own UI, before showing the system prompt.
    2) Do this as part of your "user priming" UI flow (on boarding etc) before authenticating to Neura.
    3) The prompt will be shown after Neura authentication, if it wasn't shown by you before that point in time, but for
    getting the highest conversion rates possible, it is recommended to do the above.
 */
- (void)requestMotionAuthorization;

#pragma mark - Engagement Api: User attributes
/** Use this NUserEngagementAttributes object to set user's engagement attributes.
    @see: NUserEngagementAttributes for more details about the API.
 
    Example of use:
 
        Obj-C:
        [NeuraSDK.shared.user setAttribute:@"first name" stringValue:@"John"]
        [NeuraSDK.shared.user setAttribute:@"last name" stringValue:@"Doe"]
 
        Swift:
 NeuraSDK.shared.user.setAttribute("first name", stringValue:"John")
 NeuraSDK.shared.user.setAttribute("last name", stringValue:"Doe")
 */
@property (nonatomic, nonnull) NUserEngagementAttributes* user;

@end






#pragma mark - Legacy API
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

/*
 -=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~
 Older Legacy API
 ------------------------------------------------------------------------------------------------------------
 
 Deprecated API.
 Still currently available, but planned to be deprecated.
 Please use newer API in new projects and start planning refactoring existing projects as soon as possible.
 -=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~-=~
 */


/*
 *  enables sdk configuration features from NEUFeatures enum
 *
 *  This method call overrides internal SDK configuration with 'flags' parameter (collection of bitwise values).
 */
extern NSString * _Nonnull const kNeuraSDKErrorDomain;
extern NSString * _Nonnull const kNeuraSDKErrorsArrayKey;
extern NSString * _Nonnull const NeuraSDKErrorDidReceiveNotification;
#pragma clang pop
