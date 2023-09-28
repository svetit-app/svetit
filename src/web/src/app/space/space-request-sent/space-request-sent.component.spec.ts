import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SpaceRequestSentComponent } from './space-request-sent.component';

describe('SpaceRequestSentComponent', () => {
  let component: SpaceRequestSentComponent;
  let fixture: ComponentFixture<SpaceRequestSentComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SpaceRequestSentComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(SpaceRequestSentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
